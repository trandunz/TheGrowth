// Copyright William Inman. All Rights Reserved.

#include "W_SurvivalHUD.h"

#include "ReactiveSlider.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "TheGrowth/Components/EntityComponent.h"
#include "TheGrowth/HUD/Widgets/Inventory/W_Inventory.h"
#include "TheGrowth/Pawns/SurvivalCharacter.h"
#include "TheGrowth/PlayerStates/SurvivalPlayerState.h"

void UW_SurvivalHUD::LateInitialize()
{
	if (ASurvivalCharacter* OwningPlayerPawn = GetOwningPlayerPawn<ASurvivalCharacter>())
	{
		OwningEntityComponent = OwningPlayerPawn->GetEntityComponent();
		UE_LOG(LogTemp, Warning, TEXT("Owning Entity Component Reference Assigned"));

		if (IsValid(OwningEntityComponent))
		{
			if (IsValid(HealthBar))
				HealthBar->SetSliderPercentage(OwningEntityComponent->GetTotalCurrentHealth() / OwningEntityComponent->GetTotalMaxHealth(), true);
			
			if (IsValid(StaminaBar))
				StaminaBar->SetSliderPercentage(OwningEntityComponent->GetCurrentStamina() / OwningEntityComponent->GetMaxStamina(), true);
		}
	}
}

void UW_SurvivalHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UW_SurvivalHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsValid(OwningEntityComponent))
	{
		if (IsValid(HealthBar))
			HealthBar->SetSliderPercentage(OwningEntityComponent->GetTotalCurrentHealth() / OwningEntityComponent->GetTotalMaxHealth(), true);

		if (IsValid(StaminaBar))
			StaminaBar->SetSliderPercentage(OwningEntityComponent->GetCurrentStamina() / OwningEntityComponent->GetMaxStamina(), true);
	}
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

void UW_SurvivalHUD::SetPreviewDummyCharacterReference(ADummySurvivalCharacter* DummyCharacter)
{
	InventoryMenu->SetPreviewDummyCharacterReference(DummyCharacter);
}
