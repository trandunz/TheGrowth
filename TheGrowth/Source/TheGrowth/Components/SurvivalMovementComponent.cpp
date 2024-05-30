#include "SurvivalMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

void USurvivalMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	bOrientRotationToMovement = true;
	RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	JumpZVelocity = 700.f;
	AirControl = 0.35f;
	MaxWalkSpeed = MaxJogSpeed;
	MaxWalkSpeedCrouched = MaxCrouchSpeed;
	MinAnalogWalkSpeed = 20.f;
	BrakingDecelerationWalking = 2000.f;
	BrakingDecelerationFalling = 1500.0f;
}

void USurvivalMovementComponent::StartSprint()
{
	MaxWalkSpeed = MaxSprintSpeed;
}

void USurvivalMovementComponent::EndSprint()
{
	MaxWalkSpeed = MaxJogSpeed;
}

void USurvivalMovementComponent::Crouch(bool bClientSimulation)
{
	Super::Crouch(bClientSimulation);
}

void USurvivalMovementComponent::UnCrouch(bool bClientSimulation)
{
	Super::UnCrouch(bClientSimulation);
}

void USurvivalMovementComponent::Prone()
{
}

void USurvivalMovementComponent::UnProne()
{
}