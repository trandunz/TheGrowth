// Copyright William Inman. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SurvivalHUD.generated.h"

UCLASS()
class THEGROWTH_API ASurvivalHUD : public AHUD
{
	GENERATED_BODY()
	friend class ASurvivalCharacter;
	friend class ASurvivalController;
	
protected:
	ASurvivalHUD();
	virtual void BeginPlay() override;
	virtual void LateInitialize();

protected:
	UFUNCTION()
	void ToggleInventoryMenu();
	UFUNCTION()
	void SetCrosshairVisible(bool Visible);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category= Prefabs)
	class TSubclassOf<UUserWidget> HUDWidgetClass{nullptr};

protected:
	UPROPERTY(VisibleAnywhere, Category= References)
	class UW_SurvivalHUD* HUDWidget{nullptr};
};
