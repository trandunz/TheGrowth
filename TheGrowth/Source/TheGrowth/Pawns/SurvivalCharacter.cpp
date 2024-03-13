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
#include "TheGrowth/PlayerStates/SurvivalPlayerState.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ASurvivalCharacter::ASurvivalCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;	
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
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
}

void ASurvivalCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
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
}

void ASurvivalCharacter::EndZoom()
{
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
	if (FirstPerson)
	{
		FollowCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Head"));
		FollowCamera->SetRelativeRotation({0.000000,90.000000,-90.000000});
		FollowCamera->SetRelativeLocation({0.0f, 10.0f, 0.0f});
		FollowCamera->bUsePawnControlRotation = true;
		bUseControllerRotationYaw = true;
	}
	else
	{
		FollowCamera->AttachToComponent(CameraBoom, FAttachmentTransformRules::SnapToTargetNotIncludingScale, USpringArmComponent::SocketName);
		FollowCamera->bUsePawnControlRotation = false;
		bUseControllerRotationYaw = false;
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
