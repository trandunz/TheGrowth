#include "SurvivalMovementComponent.h"

void USurvivalMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	bOrientRotationToMovement = true;
	RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	JumpZVelocity = 700.f;
	AirControl = 0.35f;
	MaxWalkSpeed = JogSpeed;
	MinAnalogWalkSpeed = 20.f;
	BrakingDecelerationWalking = 2000.f;
	BrakingDecelerationFalling = 1500.0f;
}

void USurvivalMovementComponent::StartSprint()
{
	MaxWalkSpeed = SprintSpeed;
}

void USurvivalMovementComponent::EndSprint()
{
	MaxWalkSpeed = JogSpeed;
}
