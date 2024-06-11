#include "MainMenuController.h"

void AMainMenuController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeUIOnly{});
	SetShowMouseCursor(true);
}
