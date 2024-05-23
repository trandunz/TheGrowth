#include "MainMenuHUD.h"

#include "Blueprint/UserWidget.h"
#include "Widgets/W_MainMenuHUD.h"

void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(HUDWidgetClass) == false)
		return;
	
	APlayerController* PlayerController = GetOwningPlayerController();
	if (IsValid(PlayerController) == false)
		return;

	HUDWidget = CreateWidget<UW_MainMenuHUD>(PlayerController, HUDWidgetClass);
	if (IsValid(HUDWidget) == false)
		return;

	HUDWidget->AddToViewport();
}
