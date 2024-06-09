// Copyright William Inman. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_SurvivalHUD.generated.h"

UCLASS()
class THEGROWTH_API UW_SurvivalHUD : public UUserWidget
{
	GENERATED_BODY()
	friend class ASurvivalHUD;
	friend class ASurvivalCharacter;

protected:
	virtual void LateInitialize();
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UFUNCTION()
	void ToggleInventoryMenu();
	
	UFUNCTION()
	void SetCrosshairVisible(bool Visible);

	UFUNCTION()
	void SetPreviewDummyCharacterReference(class ADummySurvivalCharacter* DummyCharacter);
	
protected: // References //
	UPROPERTY(VisibleAnywhere, Category= References)
	class UEntityComponent* OwningEntityComponent{nullptr};

protected: // Components //
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Components, meta = (BindWidget))
	class UW_Inventory* InventoryMenu{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Components, meta = (BindWidget))
	class UImage* Crosshair{nullptr};
};
