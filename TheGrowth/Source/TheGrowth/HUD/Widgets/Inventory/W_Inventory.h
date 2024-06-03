// Copyright William Inman. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_Inventory.generated.h"

UCLASS()
class THEGROWTH_API UW_Inventory : public UUserWidget
{
	GENERATED_BODY()
	friend class UW_SurvivalHUD;
	friend class ASurvivalHUD;
	friend class UInventoryComponent;
	friend class ASurvivalCharacter;
	
protected:
	UFUNCTION(BlueprintCallable)
	void SetSelectedWidgetIndex(int32 Index);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (BindWidget))
	class UW_Overall* Overall{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (BindWidget))
	class UW_Gear* Gear{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (BindWidget))
	class UW_Health* Health{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (BindWidget))
	class UW_Skills* Skills{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (BindWidget))
	class UW_Map* Map{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (BindWidget))
	class UW_Tasks* Tasks{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (BindWidget))
	class UWidgetSwitcher* MenuSelector{};
	
};
