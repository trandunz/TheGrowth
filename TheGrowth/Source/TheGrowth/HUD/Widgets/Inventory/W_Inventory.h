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
	friend class UInventoryComponent;
	friend class ASurvivalCharacter;
	
protected:
	UFUNCTION()
	class UW_InventorySlot* GetGridSlotAtIndex(int32 X, int32 Y);

	UFUNCTION()
	void SetGridSlotColor(int32 X, int32 Y, FLinearColor Color);

	UFUNCTION()
	void AddItem(int32 X, int32 Y, class UItemData* Item);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UW_InventoryItem> ItemWidgetClass{};
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "References", meta = (BindWidget))
	TMap<FVector2D, class UW_InventoryItem*> ItemIcons{};
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (BindWidget))
	class UW_InventoryGrid* InventoryGrid{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (BindWidget))
	class UCanvasPanel* Canvas{};
};
