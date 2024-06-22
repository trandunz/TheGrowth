// Copyright William Inman. All Rights Reserved.

#include "SurvivalHUD.h"

#include "Blueprint/UserWidget.h"
#include "TheGrowth/Pawns/SurvivalCharacter.h"
#include "Widgets/W_SurvivalHUD.h"
#include "Widgets/Inventory/W_Inventory.h"

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
	HUDWidget->LateInitialize();

	if (APawn* PlayerPawn = GetOwningPawn())
	{
		if (ASurvivalCharacter* CastedPawn = Cast<ASurvivalCharacter>(PlayerPawn))
		{
			CastedPawn->SetGearWidgetRef(HUDWidget->InventoryMenu->Gear);
		}
	}
}

void ASurvivalHUD::LateInitialize()
{
	if (IsValid(HUDWidget))
	{
		HUDWidget->LateInitialize();
	}
}

void ASurvivalHUD::ToggleInventoryMenu()
{
	if (IsValid(HUDWidget) == false)
		return;

	HUDWidget->ToggleInventoryMenu();
}

void ASurvivalHUD::SetCrosshairVisible(bool Visible)
{
	if (IsValid(HUDWidget) == false)
		return;

	HUDWidget->SetCrosshairVisible(Visible);
}
