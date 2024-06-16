#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TheGrowth/DataAssets/ItemData.h"
#include "W_InventorySlotCollection.generated.h"

UCLASS()
class THEGROWTH_API UW_InventorySlotCollection : public UUserWidget
{
	friend class UW_InventoryContainer;
	GENERATED_BODY()
	virtual void NativePreConstruct() override;

	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	void DrawBorderOutline(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
public:
	UFUNCTION()
	bool CanFitItem(class AItemBase* Item);

	UFUNCTION()
	FVector2D PickupItem(class AItemBase* Item);
	UFUNCTION()
	void RemoveItem(FItemStruct& Item);
	
	void PopulateSlotWithItem(FItemStruct& Item);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Inventory")
	int32 SizeX{1};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Inventory")
	int32 SizeY{1};

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Inventory")
	TSubclassOf<UUserWidget> SlotWidget{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Inventory")
	TSubclassOf<class UW_InventoryItem> InventoryItemWidget{};

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "References")
	TArray<class UW_InventorySlot*> SlotWidgets{};

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (BindWidget))
	class UUniformGridPanel* Grid{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (BindWidget))
	class UOverlay* Overlay{};
};
