#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_InventorySlotCollection.generated.h"

UCLASS()
class THEGROWTH_API UW_InventorySlotCollection : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativePreConstruct() override;

	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	void DrawBorderOutline(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Inventory")
	int32 SizeX{1};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Inventory")
	int32 SizeY{1};

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Inventory")
	TSubclassOf<UUserWidget> SlotWidget{};

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (BindWidget))
	class UUniformGridPanel* Grid{};
};
