// Copyright William Inman. All Rights Reserved.

#include "SurvivalPlayerState.h"

#include "TheGrowth/Components/EntityComponent.h"

ASurvivalPlayerState::ASurvivalPlayerState()
{
	EntityComponent = CreateDefaultSubobject<UEntityComponent>(TEXT("EntityComponent"));
}

void ASurvivalPlayerState::OffsetHealth(float Amount)
{
	EntityComponent->OffsetHealth(Amount);
}

bool ASurvivalPlayerState::IsDead() const
{
	return EntityComponent->IsDead();
}
