// Copyright William Inman. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_SurvivalHUD.generated.h"

UCLASS()
class THEGROWTH_API UW_SurvivalHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UFUNCTION()
	void UpdateHealthText();
	
protected: // References //
	UPROPERTY(VisibleAnywhere, Category= References)
	class UEntityComponent* OwningEntityComponent{nullptr};

protected: // Components //
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Components, meta = (BindWidget))
	class UTextBlock* HealthText{nullptr};
};
