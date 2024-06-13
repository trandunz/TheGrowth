#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_InventoryContainerLayout.generated.h"

UCLASS()
class THEGROWTH_API UW_InventoryContainerLayout : public UUserWidget
{
	GENERATED_BODY()
	friend class UW_InventoryContainer;

	virtual void NativeOnInitialized() override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class UW_InventorySlotCollection*> SlotCollections{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	class UW_InventorySlotCollection* SlotCollection{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	class UW_InventorySlotCollection* SlotCollection1{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	class UW_InventorySlotCollection* SlotCollection2{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	class UW_InventorySlotCollection* SlotCollection3{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	class UW_InventorySlotCollection* SlotCollection4{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	class UW_InventorySlotCollection* SlotCollection5{};
};
