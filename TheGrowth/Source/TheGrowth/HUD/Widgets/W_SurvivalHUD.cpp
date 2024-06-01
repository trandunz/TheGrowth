// Copyright William Inman. All Rights Reserved.

#include "W_SurvivalHUD.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "TheGrowth/Components/EntityComponent.h"
#include "TheGrowth/HUD/Widgets/Inventory/W_Inventory.h"
#include "TheGrowth/Pawns/SurvivalCharacter.h"
#include "TheGrowth/PlayerStates/SurvivalPlayerState.h"

void UW_SurvivalHUD::LateInitialize()
{
	if (ASurvivalPlayerState* OwningPlayerState = GetOwningPlayerState<ASurvivalPlayerState>())
	{
		OwningEntityComponent = OwningPlayerState->GetEntityComponent();
	}
}

void UW_SurvivalHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UW_SurvivalHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UW_SurvivalHUD::ToggleInventoryMenu()
{
	if (InventoryMenu->GetVisibility() == ESlateVisibility::Visible)
	{
		InventoryMenu->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		InventoryMenu->SetVisibility(ESlateVisibility::Visible);
		InventoryMenu->SetFocus();

		if (APlayerController* OwningController = GetOwningPlayer())
		{
			int32 ViewportWidth{};
			int32 ViewportHeight{};
			OwningController->GetViewportSize(ViewportWidth, ViewportHeight);
			OwningController->SetMouseLocation(ViewportWidth/2.0f, ViewportHeight/2.0f);
		}
	}
}

void UW_SurvivalHUD::SetCrosshairVisible(bool Visible)
{
	Crosshair->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}
