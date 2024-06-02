#include "W_InventorySlotCollection.h"

#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

void UW_InventorySlotCollection::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(SlotWidget) == false)
		return;
	
	if (Grid->GetSlots().Num() != SizeX * SizeY)
	{
		Grid->ClearChildren();
		for(int32 Y = 0; Y < SizeY; Y++)
			for(int32 X = 0; X < SizeX; X++)
			{
				auto NewSlot = CreateWidget(Grid, SlotWidget);
				auto GridSlot = Grid->AddChildToUniformGrid(NewSlot);
				GridSlot->SetColumn(X);
				GridSlot->SetRow(Y);
				GridSlot->SetHorizontalAlignment(HAlign_Fill);
				GridSlot->SetVerticalAlignment(VAlign_Fill);
			}
	}
}

int32 UW_InventorySlotCollection::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	DrawBorderOutline(AllottedGeometry, OutDrawElements, LayerId);
	
	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                          bParentEnabled);
}

void UW_InventorySlotCollection::DrawBorderOutline(const FGeometry& AllottedGeometry,
	FSlateWindowElementList& OutDrawElements, int32 LayerId) const
{
	if (GetDesiredSize().Length() <= 0)
		return;
	
	FVector2D TopLeft;
	TopLeft.X = 0;
	TopLeft.Y = 0;

	FVector2D BottomRight;
	BottomRight.X = GetDesiredSize().X;
	BottomRight.Y = GetDesiredSize().Y;

	TArray< FVector2D > Vertices;
	Vertices.AddUninitialized(5);
	Vertices[0] = FVector2D(TopLeft.X, TopLeft.Y);
	Vertices[1] = FVector2D(TopLeft.X, BottomRight.Y);
	Vertices[2] = FVector2D(BottomRight.X, BottomRight.Y);
	Vertices[3] = FVector2D(BottomRight.X, TopLeft.Y);
	Vertices[4] = FVector2D(TopLeft.X, TopLeft.Y);

	FSlateDrawElement::MakeLines(
	OutDrawElements,
	LayerId,
	AllottedGeometry.ToPaintGeometry(),
	Vertices,
	ESlateDrawEffect::None,
	FLinearColor(0.85f, 0.85f, 0.85f, 1.0f),
	true,
	2.0f
	);
}
