// Copyright William Inman. All Rights Reserved.

#include "SurvivalPlayerState.h"

#include "TheGrowth/Components/EntityComponent.h"

ASurvivalPlayerState::ASurvivalPlayerState()
{
	EntityComponent = CreateDefaultSubobject<UEntityComponent>(TEXT("Entity Component"));
}

void ASurvivalPlayerState::OffsetHealth(float Amount)
{
	EntityComponent->OffsetHealth(Amount);
}

void ASurvivalPlayerState::OffsetStamina(float Amount)
{
	EntityComponent->OffsetStamina(Amount);
}

bool ASurvivalPlayerState::IsDead() const
{
	return EntityComponent->IsDead();
}
