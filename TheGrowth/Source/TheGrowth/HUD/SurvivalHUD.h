// Copyright William Inman. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SurvivalHUD.generated.h"

UCLASS()
class THEGROWTH_API ASurvivalHUD : public AHUD
{
	GENERATED_BODY()
protected:
	ASurvivalHUD();
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category= Prefabs)
	class TSubclassOf<UUserWidget> HUDWidgetClass{nullptr};

protected:
	UPROPERTY(VisibleAnywhere, Category= References)
	class UW_SurvivalHUD* HUDWidget{nullptr};
};
