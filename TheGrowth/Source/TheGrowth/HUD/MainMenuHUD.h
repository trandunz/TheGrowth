#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

UCLASS()
class THEGROWTH_API AMainMenuHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category= Prefabs)
	class TSubclassOf<UUserWidget> HUDWidgetClass{nullptr};

protected:
	UPROPERTY(VisibleAnywhere, Category= References)
	class UW_MainMenuHUD* HUDWidget{nullptr};
};
