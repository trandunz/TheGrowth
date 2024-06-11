// Copyright William Inman. All Rights Reserved.

#include "W_InventoryGrid.h"

#include "Components/GridPanel.h"
#include "W_InventorySlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

void UW_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ContructGrid(10, 20);
}
int32 UW_InventoryGrid::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                    const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
                                    const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	DrawBorderOutline(AllottedGeometry, OutDrawElements, LayerId);
	DrawSlotSeperatorLines(AllottedGeometry, OutDrawElements, LayerId);

	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}

void UW_InventoryGrid::ContructGrid(int32 NewSizeX, int32 NewSizeY)
{
	if (IsValid(SlotWidget) == false)
		return;

	SizeX = NewSizeX;
	SizeY = NewSizeY;
	
	for(int32 y = 0; y < SizeY; y++)
	{
		for(int32 x = 0; x < SizeX; x++)
		{
			auto NewSlot = CreateWidget<UW_InventorySlot>(UniformGrid, SlotWidget);
			Slots.Add(NewSlot);
			
			auto Widget = UniformGrid->AddChildToUniformGrid(NewSlot, y, x);
			Widget->SetHorizontalAlignment(HAlign_Fill);
			Widget->SetVerticalAlignment(VAlign_Fill);
		}
	}
	UniformGrid->InvalidateLayoutAndVolatility();
}

class UW_InventorySlot* UW_InventoryGrid::GetSlotAtIndex(int32 X, int32 Y)
{
	if (SizeX <= X)
		return nullptr;

	if (SizeY <= Y)
		return nullptr;
	
	return Slots[(Y * SizeX) + X];
}

void UW_InventoryGrid::DrawBorderOutline(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const
{
	if (IsValid(SlotWidget) == false)
		return;

	if (IsValid(UniformGrid) == false)
		return;
	
	if (UniformGrid->GetDesiredSize().Length() <= 0)
		return;
	
	FVector2D TopLeft;
	TopLeft.X = 0;
	TopLeft.Y = 0;

	FVector2D BottomRight;
	BottomRight.X = UniformGrid->GetDesiredSize().X;
	BottomRight.Y = UniformGrid->GetDesiredSize().Y;

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

void UW_InventoryGrid::DrawSlotSeperatorLines(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const
{
	if (IsValid(SlotWidget) == false)
		return;

	if (IsValid(UniformGrid) == false)
		return;
	
	if (UniformGrid->GetDesiredSize().Length() <= 0)
		return;

	float SlotSizeX = UniformGrid->GetSlots()[0]->Content->GetDesiredSize().X;
	float SlotSizeY = UniformGrid->GetSlots()[0]->Content->GetDesiredSize().Y;
	
	for(int32 y = 0; y < SizeY; y++)
	{
		TArray<FVector2D> Vertices
		{
			{0, y * SlotSizeY},{UniformGrid->GetDesiredSize().X, y * SlotSizeY}
		};

		FSlateDrawElement::MakeLines(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(),
		Vertices,
		ESlateDrawEffect::None,
		FLinearColor(0.5f, 0.5f, 0.5f, 1.0f),
		true,
		1.0f
		);
	}

	for(int32 x = 0; x < SizeX; x++)
	{
		TArray<FVector2D> Vertices
		{
				{x * SlotSizeX, 0},{x * SlotSizeX, UniformGrid->GetDesiredSize().Y}
		};

		FSlateDrawElement::MakeLines(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(),
		Vertices,
		ESlateDrawEffect::None,
		FLinearColor(0.5f, 0.5f, 0.5f, 1.0f),
		true,
		1.0f
		);
	}
}
