// Copyright William Inman. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_InventorySlot.generated.h"

UCLASS()
class THEGROWTH_API UW_InventorySlot : public UUserWidget
{
	GENERATED_BODY()
	friend class UW_Inventory;
	friend class UW_InventoryContainer;
	friend class UW_InventorySlotCollection;
	
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	void DrawBorderOutline(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;
	
protected:
	UFUNCTION()
	void SetOverlayColor(FLinearColor Color);
	

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Overlay")
	float OverlayOpacity{0.25f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Overlay")
	bool bOccupied{};
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (BindWidget))
	class UImage* Overlay{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (BindWidget))
	class UImage* Icon{};
};
