// Copyright William Inman. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_InventoryGrid.generated.h"

UCLASS()
class THEGROWTH_API UW_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()
	friend class UW_Inventory;

	virtual void NativeOnInitialized() override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

protected:
	UFUNCTION()
	void ContructGrid(int32 SizeX, int32 SizeY);

	UFUNCTION()
	class UW_InventorySlot* GetSlotAtIndex(int32 X, int32 Y);
	
	void DrawBorderOutline(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;
	void DrawSlotSeperatorLines(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;
	
protected:
	int32 SizeX, SizeY{};

	UPROPERTY(VisibleAnywhere)
	TArray<class UW_InventorySlot*> Slots{};
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UW_InventorySlot> SlotWidget{};
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UUniformGridPanel* UniformGrid{nullptr};

};
