#include "SurvivalController.h"

void ASurvivalController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly{});
	SetShowMouseCursor(false);
}
