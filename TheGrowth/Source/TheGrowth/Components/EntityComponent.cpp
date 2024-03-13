// Copyright William Inman. All Rights Reserved.

#include "EntityComponent.h"

void UEntityComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;
}

void UEntityComponent::OffsetHealth(float Amount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0, MaxHealth);
}

bool UEntityComponent::IsDead() const
{
	return CurrentHealth <= 0;
}
