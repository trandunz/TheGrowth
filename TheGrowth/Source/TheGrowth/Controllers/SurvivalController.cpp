#include "SurvivalController.h"

#include "TheGrowth/HUD/SurvivalHUD.h"

void ASurvivalController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly{});
	SetShowMouseCursor(false);
}

void ASurvivalController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	if (ASurvivalHUD* HUD = GetHUD<ASurvivalHUD>())
		HUD->LateInitialize();
}
