// Copyright William Inman. All Rights Reserved.

#include "SurvivalHUD.h"

#include "Blueprint/UserWidget.h"
#include "TheGrowth/Widgets/W_SurvivalHUD.h"

ASurvivalHUD::ASurvivalHUD()
{
}

void ASurvivalHUD::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(HUDWidgetClass) == false)
		return;
	
	APlayerController* PlayerController = GetOwningPlayerController();
	if (IsValid(PlayerController) == false)
		return;

	HUDWidget = CreateWidget<UW_SurvivalHUD>(PlayerController, HUDWidgetClass);
	if (IsValid(HUDWidget) == false)
		return;

	HUDWidget->AddToViewport();
}
