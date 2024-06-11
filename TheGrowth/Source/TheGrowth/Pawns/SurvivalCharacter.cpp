// Copyright William Inman. All Rights Reserved.

#include "SurvivalCharacter.h"

#include "DummySurvivalCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Animation/AnimInstanceProxy.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TheGrowth/Components/InventoryComponent.h"
#include "TheGrowth/Components/ItemComponent.h"
#include "TheGrowth/Components/SurvivalMovementComponent.h"
#include "TheGrowth/DataAssets/ItemData.h"
#include "TheGrowth/HUD/SurvivalHUD.h"
#include "TheGrowth/HUD/Widgets/W_SurvivalHUD.h"
#include "TheGrowth/HUD/Widgets/Inventory/W_Inventory.h"
#include "TheGrowth/HUD/Widgets/Inventory/Tabs/W_Gear.h"
#include "TheGrowth/Interfaces/InteractInterface.h"
#include "TheGrowth/Items/ItemBase.h"
#include "TheGrowth/Items/WeaponBase.h"
#include "TheGrowth/PlayerStates/SurvivalPlayerState.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ASurvivalCharacter::ASurvivalCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	bUseControllerRotationYaw = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;	
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	ZoomTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ZoomTimeline"));
	CameraResetTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("CameraResetTimeline"));

	AimTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AimTimeline"));

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));
}

void ASurvivalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASurvivalCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASurvivalCharacter::Look);
		EnhancedInputComponent->BindAction(ScrollAction, ETriggerEvent::Triggered, this, &ASurvivalCharacter::Scroll);

		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ASurvivalCharacter::StartAim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ASurvivalCharacter::EndAim);

		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &ASurvivalCharacter::StartZoom);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Completed, this, &ASurvivalCharacter::EndZoom);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ASurvivalCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASurvivalCharacter::EndSprint);

		EnhancedInputComponent->BindAction(FreeLookAction, ETriggerEvent::Started, this, &ASurvivalCharacter::StartFreeLook);
		EnhancedInputComponent->BindAction(FreeLookAction, ETriggerEvent::Completed, this, &ASurvivalCharacter::EndFreeLook);

		EnhancedInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Completed, this, &ASurvivalCharacter::ToggleInventoryWidget);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ASurvivalCharacter::TryInteract);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ASurvivalCharacter::ToggleCrouch);

		EnhancedInputComponent->BindAction(LeanActionLeft, ETriggerEvent::Started, this, &ASurvivalCharacter::StartLean, true);
		EnhancedInputComponent->BindAction(LeanActionLeft, ETriggerEvent::Completed, this, &ASurvivalCharacter::EndLean, true);

		EnhancedInputComponent->BindAction(LeanActionRight, ETriggerEvent::Started, this, &ASurvivalCharacter::StartLean, false);
		EnhancedInputComponent->BindAction(LeanActionRight, ETriggerEvent::Completed, this, &ASurvivalCharacter::EndLean, false);

		EnhancedInputComponent->BindAction(ProneAction, ETriggerEvent::Started, this, &ASurvivalCharacter::Prone);

		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ASurvivalCharacter::StartAim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ASurvivalCharacter::EndAim);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ASurvivalCharacter::Attack);

		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ASurvivalCharacter::Reload);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASurvivalCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(Controller);
	PlayerStateRef = GetPlayerState<ASurvivalPlayerState>();
	MovementComponent = GetCharacterMovement<USurvivalMovementComponent>();
	
	MovementComponent->bOrientRotationToMovement = false;
	
	if (IsValid(PlayerController))
	{
		HUDRef = PlayerController->GetHUD<ASurvivalHUD>();
		
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			Subsystem->AddMappingContext(OOCMappingContext, 1);
		}
	}

	if (ZoomCurve)
	{
		FOnTimelineFloat ZoomCallback;
		ZoomCallback.BindDynamic(this, &ASurvivalCharacter::UpdateZoomTimeline);
		ZoomTimeline->AddInterpFloat(ZoomCurve, ZoomCallback);
		ZoomTimeline->SetPlayRate(1.0f / FMath::Clamp(ZoomTime, 0.001f, 2.0f));
	}
	if (CameraResetCurve)
	{
		FOnTimelineFloat CameraResetCallback;
		CameraResetCallback.BindDynamic(this, &ASurvivalCharacter::UpdateCameraResetTimeline);
		FOnTimelineEvent CameraResetFinishedCallback;
		CameraResetFinishedCallback.BindDynamic(this, &ASurvivalCharacter::OnCameraResetTimelineFinish);
		CameraResetTimeline->AddInterpFloat(CameraResetCurve, CameraResetCallback);
		CameraResetTimeline->SetTimelineFinishedFunc(CameraResetFinishedCallback);
		CameraResetTimeline->SetPlayRate(1.0f / FMath::Clamp(CameraResetTime, 0.001f, 2.0f));
	}
	if (AimCurve)
	{
		FOnTimelineFloat AimCallback;
		AimCallback.BindDynamic(this, &ASurvivalCharacter::UpdateAimTimeline);
		FOnTimelineEvent AimFinishedCallback;
		AimFinishedCallback.BindDynamic(this, &ASurvivalCharacter::OnAimTimelineFinished);
		AimTimeline->AddInterpFloat(AimCurve, AimCallback);
		AimTimeline->SetTimelineFinishedFunc(AimFinishedCallback);
		AimTimeline->SetPlayRate(1.0f / FMath::Clamp(AimTime, 0.001f, 2.0f));
	}

	SetPerspective(true);

	if (IsValid(TestWeaponPrefab))
	{
		auto NewWeapon = GetWorld()->SpawnActor<AWeaponBase>(TestWeaponPrefab);
		NewWeapon->TogglePhysics(false);
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("S_Firearm"));
		ActiveWeaponRef = NewWeapon;
	}

	FSlateApplication::Get().OnApplicationActivationStateChanged().AddUObject( this, &ASurvivalCharacter::OnWindowFocusChanged );

	if (IsValid(DummyCharacterPrefab))
	{
		DummyCharacter = GetWorld()->SpawnActor<ADummySurvivalCharacter>(DummyCharacterPrefab);
		DummyCharacter->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}

	if (IsValid(HUDRef))
	{
		if (IsValid(HUDRef->HUDWidget))
			HUDRef->HUDWidget->SetPreviewDummyCharacterReference(DummyCharacter);
	}
}

void ASurvivalCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckForInteractables();

	UpdateLeanAngle(DeltaSeconds);
}

void ASurvivalCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		FRotator Rotation{};
		if (bFreeLooking)
			Rotation.Yaw = FreeLookStartYaw;
		else
			Rotation = Controller->GetControlRotation();
		
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASurvivalCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (bFreeLooking)
	{
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewYawMin = FreeLookStartYaw - FreeLookYawExtent;
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewYawMax = FreeLookStartYaw + FreeLookYawExtent;
	}
	else
	{
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewYawMin = 0.0f;
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewYawMax = 359.998993f;
	}
		
	
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASurvivalCharacter::Scroll(const FInputActionValue& Value)
{
	float ScrollValue = Value.Get<float>();
	UpdateBoomLength(-ScrollValue * ScrollZoomStrength);
}

void ASurvivalCharacter::StartAim()
{
	AimTimeline->Play();
}

void ASurvivalCharacter::EndAim()
{
	AimTimeline->Reverse();
}

void ASurvivalCharacter::StartZoom()
{
	ZoomTimeline->Play();
}

void ASurvivalCharacter::EndZoom()
{
	ZoomTimeline->Reverse();
}

void ASurvivalCharacter::StartFreeLook()
{
	bFreeLooking = true;
	FreeLookStartYaw = Controller->GetControlRotation().Yaw;
	CameraResetTimeline->Stop();
	
	bUseControllerRotationYaw = false;
}

void ASurvivalCharacter::EndFreeLook()
{
	bFreeLooking = false;
	FreeLookEndYaw = Controller->GetControlRotation().Yaw;
	
	CameraResetTimeline->PlayFromStart();
}

void ASurvivalCharacter::ToggleCrouch()
{
	if (bIsCrouched)
		UnCrouch();
	else
		Crouch(true);
}

void ASurvivalCharacter::StartLean(bool Left)
{
	if (Left)
	{
		LeanInput = FMath::Clamp(LeanInput - 1, -1, 1);
	}
	else
	{
		LeanInput = FMath::Clamp(LeanInput + 1, -1, 1);
	}
}

void ASurvivalCharacter::EndLean(bool Left)
{
	if (Left)
	{
		LeanInput = FMath::Clamp(LeanInput + 1, -1, 1);
	}
	else
	{
		LeanInput = FMath::Clamp(LeanInput - 1, -1, 1);
	}
}

void ASurvivalCharacter::Jump()
{
	UE_LOG(LogTemp, Warning, TEXT("Attempted Jump") );
	
	if (IsValid(MovementComponent))
	{
		if (MovementComponent->IsMovementMode(MOVE_Custom) && MovementComponent->IsCustomMovementMode(ECustomMovementMode::CMOVE_Prone))
		{
			Crouch(true);
		}
	}
	
	Super::Jump();
}

void ASurvivalCharacter::Prone()
{
	if (IsValid(MovementComponent) == false)
		return;

	MovementComponent->Prone();
}

void ASurvivalCharacter::StartSprint()
{
	if (IsValid(MovementComponent) == false)
		return;

	MovementComponent->StartSprint();
}

void ASurvivalCharacter::EndSprint()
{
	if (IsValid(MovementComponent) == false)
		return;

	MovementComponent->EndSprint();
}

void ASurvivalCharacter::OffsetHealth(float Amount)
{
	if (PlayerStateRef->IsDead() == false)
	{
		PlayerStateRef->OffsetHealth(Amount);

		// Check If It Killed Player //
		if (PlayerStateRef->IsDead())
		{
			SetRagdoll(true);
		}
	}
}

FTransform ASurvivalCharacter::GetLeftHandSocketTransform()
{
	return{};
}

void ASurvivalCharacter::OnWindowFocusChanged(bool bIsFocused)
{
	if (bIsFocused == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Window Lost Focus") );
		//EndFreeLook();
	}
		
}

void ASurvivalCharacter::UpdateBoomLength(float Increment)
{
	float NewArmLength = CameraBoom->TargetArmLength + Increment;
	
	if (CameraBoom->TargetArmLength == MinBoomLength &&
		Increment < 0 &&
		FollowCamera->GetAttachParent() == CameraBoom)
	{
		SetPerspective(true);
	}
	else if (CameraBoom->TargetArmLength == MinBoomLength &&
		Increment > 0 &&
		FollowCamera->GetAttachParent() != CameraBoom)
	{
		SetPerspective(false);
	}
	else
	{
		CameraBoom->TargetArmLength = FMath::Clamp(NewArmLength, MinBoomLength, MaxBoomLength);
	}
}

void ASurvivalCharacter::SetPerspective(bool FirstPerson)
{
	bFirstPerson = FirstPerson;
	if (FirstPerson)
	{
		FollowCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("S_Camera"));

		CameraBoom->TargetArmLength = MinBoomLength;
		
		FollowCamera->bUsePawnControlRotation = true;
		CameraBoom->bUsePawnControlRotation = false;

		if (bFreeLooking == false)
			bUseControllerRotationYaw = true;
	}
	else
	{
		FollowCamera->AttachToComponent(CameraBoom, FAttachmentTransformRules::SnapToTargetNotIncludingScale, USpringArmComponent::SocketName);
		
		FollowCamera->bUsePawnControlRotation = false;
		CameraBoom->bUsePawnControlRotation = true;
		
		if (bFreeLooking == false)
			bUseControllerRotationYaw = true;
	}
}

void ASurvivalCharacter::SetRagdoll(bool Ragdoll)
{
	if (Ragdoll)
	{
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->WakeAllRigidBodies();
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

		if (IsValid(PlayerController))
		{
			PlayerController->SetIgnoreMoveInput(true);
		}
	}
	else
	{
		GetMesh()->SetAllBodiesSimulatePhysics(false);
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->WakeAllRigidBodies();
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		
		if (IsValid(PlayerController))
		{
			PlayerController->SetIgnoreMoveInput(false);
		}
	}
}

void ASurvivalCharacter::UpdateZoomTimeline(float Delta)
{
	FollowCamera->SetFieldOfView(FMath::Lerp(DefaultFOV, ZoomFOV, Delta));
}

void ASurvivalCharacter::UpdateCameraResetTimeline(float Delta)
{
	FRotator ControlRotation = Controller->GetControlRotation();
	FRotator FreeLookEndRotation = FRotator{ControlRotation.Pitch, FreeLookEndYaw, ControlRotation.Roll};
	FRotator FreeLookStartRotation = FRotator{ControlRotation.Pitch, FreeLookStartYaw, ControlRotation.Roll};
	ControlRotation = FQuat::Slerp(FreeLookEndRotation.Quaternion(), FreeLookStartRotation.Quaternion(), Delta).Rotator();
	Controller->SetControlRotation(ControlRotation);
}

void ASurvivalCharacter::OnCameraResetTimelineFinish()
{
	bUseControllerRotationYaw = true;
	UE_LOG(LogTemp, Warning, TEXT("Reset Camera Complete") );
}

void ASurvivalCharacter::UpdateLeanAngle(float DeltaSeconds)
{
	if (LeanInput == 0)
	{
		if (LeanAngle < 0)
		{
			LeanAngle = FMath::Clamp(LeanAngle + DeltaSeconds * LeanSpeed, -MaximumLeanAngle, 0);
		}
		else if (LeanAngle > 0)
		{
			LeanAngle = FMath::Clamp(LeanAngle - DeltaSeconds * LeanSpeed, 0, MaximumLeanAngle);
		}
	}
	else if (LeanInput < 0)
	{
		LeanAngle = FMath::Clamp(LeanAngle - DeltaSeconds * LeanSpeed, -MaximumLeanAngle, MaximumLeanAngle);
	}
	else if (LeanInput > 0)
	{
		LeanAngle = FMath::Clamp(LeanAngle + DeltaSeconds * LeanSpeed, -MaximumLeanAngle, MaximumLeanAngle);
	}
}

void ASurvivalCharacter::ToggleInventoryWidget()
{
	if (IsValid(HUDRef) == false)
		return;

	HUDRef->ToggleInventoryMenu();

	if (IsValid(PlayerController) == false)
		return;

	DisableInput(PlayerController);
	EnableInput(PlayerController);
	PlayerController->SetInputMode(FInputModeUIOnly{});
}

void ASurvivalCharacter::SetGearWidgetRef(UW_Gear* GearWidget)
{
	GearRef = GearWidget;
}

void ASurvivalCharacter::CheckForInteractables()
{
	FVector TraceStart = FollowCamera->GetComponentLocation();
	FVector TraceEnd = TraceStart + FollowCamera->GetForwardVector() * InteractRange;
	
	FCollisionQueryParams TraceParams{};
	TraceParams.AddIgnoredActor(this);
	TArray<FHitResult> HitResults{};
	
	if (!GetWorld()->LineTraceMultiByChannel(HitResults, TraceStart, TraceEnd, ECC_Visibility, TraceParams))
	{
		if (IsValid(HUDRef))
			HUDRef->SetCrosshairVisible(false);

		LastInteractable = nullptr;
		return;
	}

	TArray<AActor*> InteractableActors{};
	for(auto Result : HitResults)
	{
		if (auto Interactable = Cast<IInteractInterface>(Result.GetActor()))
		{
			InteractableActors.Add(Result.GetActor());
		}
	}

	if (InteractableActors.Num() <= 0)
	{
		if (IsValid(HUDRef))
			HUDRef->SetCrosshairVisible(false);

		LastInteractable = nullptr;
		return;
	}
	
	float ClosestDistance{};
	AActor* ClosestInteractableActor = UGameplayStatics::FindNearestActor(TraceStart, InteractableActors, ClosestDistance);
	IInteractInterface* ClosestInteractable = Cast<IInteractInterface>(ClosestInteractableActor);

	if (ClosestInteractable != LastInteractable)
	{
		if (IsValid(HUDRef))
			HUDRef->SetCrosshairVisible(true);

		LastInteractable = ClosestInteractable;
	}
}

void ASurvivalCharacter::TryInteract()
{
	if (LastInteractable == nullptr)
		return;

	LastInteractable->Interact(this);
}

void ASurvivalCharacter::PickupItem(AItemBase* Item)
{
	if (IsValid(Item) == false)
		return;

	InventoryComponent->AddItem(Item);
	Item->Destroy();
}

void ASurvivalCharacter::UpdateAimTimeline(float Delta)
{
	AimDelta = Delta;
}

void ASurvivalCharacter::OnAimTimelineFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("Aim Finished") );
}

void ASurvivalCharacter::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attempted To Fire Gun") );
	
	if (IsValid(ActiveWeaponRef) == false)
		return;

	ActiveWeaponRef->Discharge();
}

void ASurvivalCharacter::Reload()
{
	if (IsValid(ActiveWeaponRef) == false)
		return;

	for(FItemStruct Item : InventoryComponent->Inventory)
	{
		if (Item.ItemData->ItemType == Magazine)
		{
			if (ActiveWeaponRef->CanFitMagazine(Item.ItemData))
			{
				ActiveWeaponRef->Reload(Item);
				break;
			}
		}
	}
}
