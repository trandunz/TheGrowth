#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TheGrowth/DataAssets/ItemData.h"
#include "W_InventoryContainer.generated.h"

UCLASS()
class THEGROWTH_API UW_InventoryContainer : public UUserWidget
{
	GENERATED_BODY()
	friend class UW_Gear;

	virtual void NativePreConstruct() override;

public:
	UFUNCTION()
	bool IsOccupied() const;

	UFUNCTION()
	bool CanFitItem(class AItemBase* Item);
	
protected:
	UFUNCTION()
	void UpdateContainer();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Inventory")
	bool bOccupied{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Inventory")
	TEnumAsByte<EItemType> SlotType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Inventory")
	class UInventoryContainerData* ContainerData{};
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "References")
	class UW_InventoryContainerLayout* LayoutWidget{};
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (BindWidget))
	class UNamedSlot* ContainerWidget{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (BindWidgetOptional))
	class UW_InventorySlotCollection* ItemSlot{};

};
