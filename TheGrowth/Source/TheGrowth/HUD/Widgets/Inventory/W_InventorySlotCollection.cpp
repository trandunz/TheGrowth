#include "W_InventorySlotCollection.h"

#include "W_InventorySlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "TheGrowth/DataAssets/ItemData.h"
#include "TheGrowth/Items/ItemBase.h"

void UW_InventorySlotCollection::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(SlotWidget) == false)
		return;
	
	if (Grid->GetSlots().Num() != SizeX * SizeY)
	{
		Grid->ClearChildren();
		SlotWidgets.Empty();
		for(int32 Y = 0; Y < SizeY; Y++)
			for(int32 X = 0; X < SizeX; X++)
			{
				const auto NewSlot = CreateWidget(Grid, SlotWidget);
				SlotWidgets.Add(Cast<UW_InventorySlot>(NewSlot));
				const auto GridSlot = Grid->AddChildToUniformGrid(NewSlot);
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

bool UW_InventorySlotCollection::CanFitItem(AItemBase* Item)
{
	for(int32 SlotY = 0; SlotY < SizeY; SlotY++)
	{
		for(int32 SlotX = 0; SlotX < SizeX; SlotX++)
		{
			// Horizontal
			bool ItemCanFitHere{true};
			for(int32 ItemY = 0; ItemY < Item->ItemData->SizeY; ItemY++)
			{
				for(int32 ItemX = 0; ItemX < Item->ItemData->SizeX; ItemX++)
				{
					if (ItemY + SlotY >= SizeY)
					{
						ItemCanFitHere = false;
						continue;
					}
					if (ItemX + SlotX >= SizeX)
					{
						ItemCanFitHere = false;
						continue;
					}
					int32 NewIndex = ((SlotY + ItemY) * SizeX) + (SlotX + ItemX);
					if (SlotWidgets[NewIndex]->bOccupied)
					{
						ItemCanFitHere = false;
						continue;
					}
				}
			}
			if (ItemCanFitHere)
			{
				return true;
			}

			// Vertical
			ItemCanFitHere = true;
			for(int32 ItemY = 0; ItemY < Item->ItemData->SizeY; ItemY++)
			{
				for(int32 ItemX = 0; ItemX < Item->ItemData->SizeX; ItemX++)
				{
					if (ItemX + SlotY >= SizeY)
					{
						ItemCanFitHere = false;
						continue;
					}
					if (ItemY + SlotX >= SizeX)
					{
						ItemCanFitHere = false;
						continue;
					}
					int32 NewIndex = ((SlotY + ItemX) * SizeX) + (SlotX + ItemY);
					if (SlotWidgets[NewIndex]->bOccupied)
					{
						ItemCanFitHere = false;
						continue;
					}
				}
			}
			if (ItemCanFitHere)
			{
				return true;
			}
		}
	}

	return false;
}
