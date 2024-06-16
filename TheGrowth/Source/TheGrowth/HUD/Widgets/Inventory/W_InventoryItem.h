#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "TheGrowth/DataAssets/ItemData.h"
#include "TheGrowth/HUD/Widgets/GenericDragDropOperation.h"
#include "W_InventoryItem.generated.h"

UCLASS()
class THEGROWTH_API UW_InventoryItem : public UUserWidget
{
	GENERATED_BODY()
	friend class UW_Inventory;
	friend class UW_InventorySlotCollection;

protected:
	virtual void NativeOnInitialized() override;
	
protected:
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	void DrawBorderOutline(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;

protected:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FItemStruct ItemStruct{};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class ASurvivalCharacter* CharacterReference{};
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Components", meta = (BindWidget))
	class UImage* Icon{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Components", meta = (BindWidget))
	class USizeBox* SizeBox{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Components", meta = (BindWidget))
	class UBorder* BG{};
};
