// Copyright William Inman. All Rights Reserved.

#include "SurvivalCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TheGrowth/Components/SurvivalMovementComponent.h"
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
}

void ASurvivalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UCharacterMovementComponent* BaseMovementComponent = GetCharacterMovement())
		if (IsValid(BaseMovementComponent))
			if (USurvivalMovementComponent* SurvivalMovementComponent = Cast<USurvivalMovementComponent>(BaseMovementComponent))
				if (IsValid(SurvivalMovementComponent))
					MovementComponent = SurvivalMovementComponent;
	
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

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, MovementComponent, &USurvivalMovementComponent::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, MovementComponent, &USurvivalMovementComponent::EndSprint);

		EnhancedInputComponent->BindAction(FreeLookAction, ETriggerEvent::Started, this, &ASurvivalCharacter::StartFreeLook);
		EnhancedInputComponent->BindAction(FreeLookAction, ETriggerEvent::Completed, this, &ASurvivalCharacter::EndFreeLook);
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
	
	if (IsValid(PlayerController))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
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
}

void ASurvivalCharacter::EndAim()
{
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
	MovementComponent->bOrientRotationToMovement = false;
}

void ASurvivalCharacter::EndFreeLook()
{
	bFreeLooking = false;
	FreeLookEndYaw = Controller->GetControlRotation().Yaw;
	
	CameraResetTimeline->PlayFromStart();
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
		FollowCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Head"));
		FollowCamera->SetRelativeRotation({0.000000,90.000000,-90.000000});
		FollowCamera->SetRelativeLocation({0.0f, 10.0f, 0.0f});
		
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
	MovementComponent->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = true;
}
