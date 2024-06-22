// Copyright William Inman. All Rights Reserved.

#include "EntityComponent.h"

UEntityComponent::UEntityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEntityComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;
}

void UEntityComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RegenerateStamina(DeltaTime);
}

void UEntityComponent::OffsetHealth(float Amount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0, MaxHealth);
}

void UEntityComponent::OffsetStamina(float Amount)
{
	bShouldRegenStamina = false;
	CurrentStamina = FMath::Clamp(CurrentStamina + Amount, 0, MaxStamina);
	GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimerHandle, this, &UEntityComponent::BeginStaminaReset, StaminaRegenerationDelay);
}

bool UEntityComponent::IsDead() const
{
	return CurrentHealth <= 0;
}

void UEntityComponent::BeginStaminaReset()
{
	bShouldRegenStamina = true;
}

void UEntityComponent::RegenerateStamina(float DeltaSeconds)
{
	if (bShouldRegenStamina == false)
		return;

	CurrentStamina = FMath::Clamp(CurrentStamina + (DeltaSeconds * StaminaRegenerationSpeed), 0, MaxStamina);
}
