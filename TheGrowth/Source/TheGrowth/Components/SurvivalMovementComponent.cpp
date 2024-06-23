#include "SurvivalMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

void USurvivalMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	bOrientRotationToMovement = false;
	RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	JumpZVelocity = 700.f;
	AirControl = 0.35f;
	MaxWalkSpeed = MaxJogSpeed;
	MaxWalkSpeedCrouched = MaxCrouchSpeed;
	MinAnalogWalkSpeed = 20.f;
	BrakingDecelerationWalking = 2000.f;
	BrakingDecelerationFalling = 1500.0f;
}

void USurvivalMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void USurvivalMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool USurvivalMovementComponent::IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}

bool USurvivalMovementComponent::IsMovementMode(EMovementMode InMovementMode) const
{
	return InMovementMode == MovementMode;
}

bool USurvivalMovementComponent::IsSprinting() const
{
	return Safe_bWantsToSprint;
}

bool USurvivalMovementComponent::CanCrouchInCurrentState() const
{
	return Super::CanCrouchInCurrentState() && IsMovingOnGround();
}

float USurvivalMovementComponent::GetMaxSpeed() const
{
	if (IsMovementMode(MOVE_Walking) && Safe_bWantsToSprint && !IsCrouching())
		return MaxSprintSpeed;
	
	if (MovementMode != MOVE_Custom)
		return Super::GetMaxSpeed();

	switch (CustomMovementMode)
	{
	case CMOVE_Prone:
		return MaxProneSpeed;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"))
		return -1.f;
	}
}

void USurvivalMovementComponent::StartSprint()
{
	Safe_bWantsToSprint = true;
}

void USurvivalMovementComponent::EndSprint()
{
	Safe_bWantsToSprint = false;
}

void USurvivalMovementComponent::Crouch(bool bClientSimulation)
{
	Super::Crouch(bClientSimulation);
}

void USurvivalMovementComponent::UnCrouch(bool bClientSimulation)
{
	Super::UnCrouch(bClientSimulation);
}

void USurvivalMovementComponent::Server_EnterProne_Implementation()
{
	Safe_bWantsToProne = true;
}

void USurvivalMovementComponent::Prone()
{
	Safe_bWantsToProne = true;
}

void USurvivalMovementComponent::EnterProne()
{
	bWantsToCrouch = true;

	FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, true, nullptr);
}

void USurvivalMovementComponent::ExitProne()
{
}

bool USurvivalMovementComponent::CanProne() const
{
	return IsMovementMode(MOVE_Walking) && IsCrouching();
}

void USurvivalMovementComponent::PhysProne(float DeltaSeconds, int32 Iterations)
{
	if (DeltaSeconds < MIN_TICK_TIME)
	{
		return;
	}

	if (!CharacterOwner || (!CharacterOwner->Controller && !bRunPhysicsWithNoController && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() && (CharacterOwner->GetLocalRole() != ROLE_SimulatedProxy)))
	{
		Acceleration = FVector::ZeroVector;
		Velocity = FVector::ZeroVector;
		return;
	}

	bJustTeleported = false;
	bool bCheckedFall = false;
	bool bTriedLedgeMove = false;
	float remainingTime = DeltaSeconds;

	// Perform the move
	while ( (remainingTime >= MIN_TICK_TIME) && (Iterations < MaxSimulationIterations) && CharacterOwner && (CharacterOwner->Controller || bRunPhysicsWithNoController || (CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)) )
	{
		Iterations++;
		bJustTeleported = false;
		const float timeTick = GetSimulationTimeStep(remainingTime, Iterations);
		remainingTime -= timeTick;

		// Save current values
		UPrimitiveComponent * const OldBase = GetMovementBase();
		const FVector PreviousBaseLocation = (OldBase != NULL) ? OldBase->GetComponentLocation() : FVector::ZeroVector;
		const FVector OldLocation = UpdatedComponent->GetComponentLocation();
		const FFindFloorResult OldFloor = CurrentFloor;

		// Ensure velocity is horizontal.
		MaintainHorizontalGroundVelocity();
		const FVector OldVelocity = Velocity;
		Acceleration.Z = 0.f;

		// Apply acceleration
		CalcVelocity(timeTick, GroundFriction, false, GetMaxBrakingDeceleration());
		
		// Compute move parameters
		const FVector MoveVelocity = Velocity;
		const FVector Delta = timeTick * MoveVelocity; // dx = v * dt
		const bool bZeroDelta = Delta.IsNearlyZero();
		FStepDownResult StepDownResult;

		if ( bZeroDelta )
		{
			remainingTime = 0.f;
		}
		else
		{
			MoveAlongFloor(MoveVelocity, timeTick, &StepDownResult);

			if ( IsFalling() )
			{
				// pawn decided to jump up
				const float DesiredDist = Delta.Size();
				if (DesiredDist > KINDA_SMALL_NUMBER)
				{
					const float ActualDist = (UpdatedComponent->GetComponentLocation() - OldLocation).Size2D();
					remainingTime += timeTick * (1.f - FMath::Min(1.f,ActualDist/DesiredDist));
				}
				StartNewPhysics(remainingTime,Iterations);
				return;
			}
			else if ( IsSwimming() ) //just entered water
			{
				StartSwimming(OldLocation, OldVelocity, timeTick, remainingTime, Iterations);
				return;
			}
		}

		// Update floor.
		// StepUp might have already done it for us.
		if (StepDownResult.bComputedFloor)
		{
			CurrentFloor = StepDownResult.FloorResult;
		}
		else
		{
			FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, bZeroDelta, NULL);
		}


		// check for ledges here
		const bool bCheckLedges = !CanWalkOffLedges();
		if ( bCheckLedges && !CurrentFloor.IsWalkableFloor() )
		{
			// calculate possible alternate movement
			const FVector GravDir = FVector(0.f,0.f,-1.f);
			const FVector NewDelta = bTriedLedgeMove ? FVector::ZeroVector : GetLedgeMove(OldLocation, Delta, GravDir);
			if ( !NewDelta.IsZero() )
			{
				// first revert this move
				RevertMove(OldLocation, OldBase, PreviousBaseLocation, OldFloor, false);

				// avoid repeated ledge moves if the first one fails
				bTriedLedgeMove = true;

				// Try new movement direction
				Velocity = NewDelta/timeTick; // v = dx/dt
				remainingTime += timeTick;
				continue;
			}
			else
			{
				// see if it is OK to jump
				// @todo collision : only thing that can be problem is that oldbase has world collision on
				bool bMustJump = bZeroDelta || (OldBase == NULL || (!OldBase->IsQueryCollisionEnabled() && MovementBaseUtility::IsDynamicBase(OldBase)));
				if ( (bMustJump || !bCheckedFall) && CheckFall(OldFloor, CurrentFloor.HitResult, Delta, OldLocation, remainingTime, timeTick, Iterations, bMustJump) )
				{
					return;
				}
				bCheckedFall = true;

				// revert this move
				RevertMove(OldLocation, OldBase, PreviousBaseLocation, OldFloor, true);
				remainingTime = 0.f;
				break;
			}
		}
		else
		{
			// Validate the floor check
			if (CurrentFloor.IsWalkableFloor())
			{
				AdjustFloorHeight();
				SetBase(CurrentFloor.HitResult.Component.Get(), CurrentFloor.HitResult.BoneName);
			}
			else if (CurrentFloor.HitResult.bStartPenetrating && remainingTime <= 0.f)
			{
				// The floor check failed because it started in penetration
				// We do not want to try to move downward because the downward sweep failed, rather we'd like to try to pop out of the floor.
				FHitResult Hit(CurrentFloor.HitResult);
				Hit.TraceEnd = Hit.TraceStart + FVector(0.f, 0.f, MAX_FLOOR_DIST);
				const FVector RequestedAdjustment = GetPenetrationAdjustment(Hit);
				ResolvePenetration(RequestedAdjustment, Hit, UpdatedComponent->GetComponentQuat());
				bForceNextFloorCheck = true;
			}

			// check if just entered water
			if ( IsSwimming() )
			{
				StartSwimming(OldLocation, Velocity, timeTick, remainingTime, Iterations);
				return;
			}

			// See if we need to start falling.
			if (!CurrentFloor.IsWalkableFloor() && !CurrentFloor.HitResult.bStartPenetrating)
			{
				const bool bMustJump = bJustTeleported || bZeroDelta || (OldBase == NULL || (!OldBase->IsQueryCollisionEnabled() && MovementBaseUtility::IsDynamicBase(OldBase)));
				if ((bMustJump || !bCheckedFall) && CheckFall(OldFloor, CurrentFloor.HitResult, Delta, OldLocation, remainingTime, timeTick, Iterations, bMustJump) )
				{
					return;
				}
				bCheckedFall = true;
			}
		}
		
		// Allow overlap events and such to change physics state and velocity
		if (IsMovingOnGround())
		{
			// Make velocity reflect actual move
			if( !bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() && timeTick >= MIN_TICK_TIME)
			{
				// TODO-RootMotionSource: Allow this to happen during partial override Velocity, but only set allowed axes?
				Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / timeTick; // v = dx / dt
				MaintainHorizontalGroundVelocity();
			}
		}

		// If we didn't move at all this iteration then abort (since future iterations will also be stuck).
		if (UpdatedComponent->GetComponentLocation() == OldLocation)
		{
			remainingTime = 0.f;
			break;
		}
	}

	if (IsMovingOnGround())
	{
		MaintainHorizontalGroundVelocity();
	}
}

bool USurvivalMovementComponent::FSavedMove_Survival::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
	FSavedMove_Survival* CastedNewMove = static_cast<FSavedMove_Survival*>(NewMove.Get());
	if (Saved_bWantsToSprint != CastedNewMove->Saved_bWantsToSprint)
	{
		return false;
	}
	
	return Super::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void USurvivalMovementComponent::FSavedMove_Survival::Clear()
{
	Super::Clear();

	Saved_bWantsToSprint = 0;
}

uint8 USurvivalMovementComponent::FSavedMove_Survival::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (Saved_bWantsToSprint)
		Result |= FLAG_Custom_0;
	
	return Result;
}

void USurvivalMovementComponent::FSavedMove_Survival::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	USurvivalMovementComponent* MovementComponent = Cast<USurvivalMovementComponent>(C->GetCharacterMovement());
	
	Saved_bWantsToSprint = MovementComponent->Safe_bWantsToSprint;
	Saved_bWantsToProne = MovementComponent->Safe_bWantsToProne;
}

void USurvivalMovementComponent::FSavedMove_Survival::PrepMoveFor(ACharacter* C)
{
	Super::PrepMoveFor(C);

	USurvivalMovementComponent* MovementComponent = Cast<USurvivalMovementComponent>(C->GetCharacterMovement());
	
	MovementComponent->Safe_bWantsToSprint = Saved_bWantsToSprint;
	MovementComponent->Safe_bWantsToProne = Saved_bWantsToProne;
}

USurvivalMovementComponent::FNetworkPredictionData_Client_Survival::FNetworkPredictionData_Client_Survival(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement) // Initialize Super Constructor
{
}

FSavedMovePtr USurvivalMovementComponent::FNetworkPredictionData_Client_Survival::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_Survival);
}

FNetworkPredictionData_Client* USurvivalMovementComponent::GetPredictionData_Client() const
{
	if(PawnOwner == nullptr)
		return ClientPredictionData;

	if (ClientPredictionData == nullptr)
	{
		USurvivalMovementComponent* MutableThis = const_cast<USurvivalMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Survival(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.0f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.0f;
	}
	
	return ClientPredictionData;
}

void USurvivalMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	Safe_bWantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

bool USurvivalMovementComponent::IsMovingOnGround() const
{
	return Super::IsMovingOnGround() || IsCustomMovementMode(ECustomMovementMode::CMOVE_Prone);
}

void USurvivalMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	// Prone
	if (Safe_bWantsToProne) 
	{
		if (CanProne())
		{
			SetMovementMode(MOVE_Custom, CMOVE_Prone);
			if (!CharacterOwner->HasAuthority()) Server_EnterProne();
		}
		Safe_bWantsToProne = false;
	}
	if (IsCustomMovementMode(CMOVE_Prone) && !bWantsToCrouch)
	{
		SetMovementMode(MOVE_Walking);
	}
	
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}

void USurvivalMovementComponent::UpdateCharacterStateAfterMovement(float DeltaSeconds)\
{
	
	Super::UpdateCharacterStateAfterMovement(DeltaSeconds);
}

void USurvivalMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if (MovementMode == MOVE_Walking)
	{
		if (Safe_bWantsToSprint)
			MaxWalkSpeed = MaxSprintSpeed;
		else
			MaxWalkSpeed = MaxJogSpeed;
	}
}

void USurvivalMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	switch (CustomMovementMode)
	{
	case CMOVE_Prone:
		PhysProne(deltaTime, Iterations);
		break;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"))
	}
}

void USurvivalMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == CMOVE_Prone)
		ExitProne();

	if (IsCustomMovementMode(CMOVE_Prone))
		EnterProne();
}
