// Copyright William Inman. All Rights Reserved.

#include "W_SurvivalHUD.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "TheGrowth/Components/EntityComponent.h"
#include "TheGrowth/HUD/Widgets/Inventory/W_Inventory.h"
#include "TheGrowth/Pawns/SurvivalCharacter.h"
#include "TheGrowth/PlayerStates/SurvivalPlayerState.h"

void UW_SurvivalHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ASurvivalPlayerState* OwningPlayerState = GetOwningPlayerState<ASurvivalPlayerState>())
	{
		OwningEntityComponent = OwningPlayerState->GetEntityComponent();
	}
}

void UW_SurvivalHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateHealthText();
}

void UW_SurvivalHUD::UpdateHealthText()
{
	if (IsValid(OwningEntityComponent) == false)
		return;

	FString HealthString = FString::SanitizeFloat(OwningEntityComponent->GetCurrentHealth(), 0);
	HealthString += '/';
	HealthString += FString::SanitizeFloat(OwningEntityComponent->GetMaxHealth(), 0);
	
	HealthText->SetText(FText::FromString(HealthString));
}

void UW_SurvivalHUD::ToggleInventoryMenu()
{
	if (InventoryMenu->GetVisibility() == ESlateVisibility::Visible)
		InventoryMenu->SetVisibility(ESlateVisibility::Collapsed);
	else
	{
		InventoryMenu->SetVisibility(ESlateVisibility::Visible);
		InventoryMenu->SetFocus();
	}
}

void UW_SurvivalHUD::SetCrosshairVisible(bool Visible)
{
	Crosshair->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}
