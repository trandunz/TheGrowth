#include "W_InventorySlotCollection.h"

#include "W_InventoryItem.h"
#include "W_InventorySlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "TheGrowth/Components/ItemComponent.h"
#include "TheGrowth/DataAssets/ItemData.h"
#include "W_InventoryItem.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/SizeBox.h"
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
				const auto NewSlot = CreateWidget(GetOwningPlayer(), SlotWidget);
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
			for(int32 ItemY = 0; ItemY < Item->ItemComponent->ItemStruct.ItemData->SizeY; ItemY++)
			{
				for(int32 ItemX = 0; ItemX < Item->ItemComponent->ItemStruct.ItemData->SizeX; ItemX++)
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
			for(int32 ItemY = 0; ItemY < Item->ItemComponent->ItemStruct.ItemData->SizeY; ItemY++)
			{
				for(int32 ItemX = 0; ItemX < Item->ItemComponent->ItemStruct.ItemData->SizeX; ItemX++)
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

FVector2D UW_InventorySlotCollection::PickupItem(AItemBase* Item)
{
	for(int32 SlotY = 0; SlotY < SizeY; SlotY++)
	{
		for(int32 SlotX = 0; SlotX < SizeX; SlotX++)
		{
			// Vertical 
			bool ItemCanFitHere{true};
			for(int32 ItemY = 0; ItemY < Item->ItemComponent->ItemStruct.ItemData->SizeY; ItemY++)
			{
				for(int32 ItemX = 0; ItemX < Item->ItemComponent->ItemStruct.ItemData->SizeX; ItemX++)
				{
					if (ItemY + SlotY >= SizeY)
					{
						UE_LOG(LogTemp, Warning, TEXT("Item Too Big Y Vert") );
						ItemCanFitHere = false;
						continue;
					}
					if (ItemX + SlotX >= SizeX)
					{
						UE_LOG(LogTemp, Warning, TEXT("Item Too Big X Vert") );
						ItemCanFitHere = false;
						continue;
					}
					int32 NewIndex = ((SlotY + ItemY) * SizeX) + (SlotX + ItemX);
					if (SlotWidgets[NewIndex]->bOccupied)
					{
						UE_LOG(LogTemp, Warning, TEXT("Item Slot Occupied Vert") );
						ItemCanFitHere = false;
						continue;
					}
				}
			}
			if (ItemCanFitHere)
			{
				UE_LOG(LogTemp, Warning, TEXT("Valid Item Slot Identified Vert") );
				PopulateSlotWithItem({(double)SlotX, (double)SlotY}, Item, false);
				return {(double)SlotX, (double)SlotY};
			}

			// Horizontal 
			ItemCanFitHere = true;
			for(int32 ItemY = 0; ItemY < Item->ItemComponent->ItemStruct.ItemData->SizeX; ItemY++)
			{
				for(int32 ItemX = 0; ItemX < Item->ItemComponent->ItemStruct.ItemData->SizeY; ItemX++)
				{
					if (ItemY + SlotY >= SizeY)
					{
						UE_LOG(LogTemp, Warning, TEXT("Item Too Big Y Vert") );
						ItemCanFitHere = false;
						continue;
					}
					if (ItemX + SlotX >= SizeX)
					{
						UE_LOG(LogTemp, Warning, TEXT("Item Too Big X Vert") );
						ItemCanFitHere = false;
						continue;
					}
					int32 NewIndex = ((SlotY + ItemY) * SizeX) + (SlotX + ItemX);
					if (SlotWidgets[NewIndex]->bOccupied)
					{
						UE_LOG(LogTemp, Warning, TEXT("Item Slot Occupied Vert") );
						ItemCanFitHere = false;
						continue;
					}
				}
			}
			if (ItemCanFitHere)
			{
				UE_LOG(LogTemp, Warning, TEXT("Valid Item Slot Identified Vert") );
				PopulateSlotWithItem({(double)SlotX, (double)SlotY}, Item, false);
				Item->ItemComponent->ItemStruct.bRotated = true;
				return {(double)SlotX, (double)SlotY};
			}
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Item Could Not Fit") );
	return {-1, -1};
}

void UW_InventorySlotCollection::RemoveItem(FItemStruct& Item)
{
	if (Item.bRotated)
	{
		if (Item.LocationInfo.Get<2>().X != -1)
		{
			for(int32 ItemY = 0; ItemY < Item.ItemData->SizeX; ItemY++)
			{
				for(int32 ItemX = 0; ItemX < Item.ItemData->SizeY; ItemX++)
				{
					int32 NewIndex = ((Item.LocationInfo.Get<2>().Y + ItemY) * SizeX) + (Item.LocationInfo.Get<2>().X + ItemX);
					SlotWidgets[NewIndex]->bOccupied = false;
					SlotWidgets[NewIndex]->SetOverlayColor(FLinearColor::Transparent);
				}
			}
		}
	}
	else
	{
		if (Item.LocationInfo.Get<2>().X != -1)
		{
			for(int32 ItemY = 0; ItemY < Item.ItemData->SizeY; ItemY++)
			{
				for(int32 ItemX = 0; ItemX < Item.ItemData->SizeX; ItemX++)
				{
					int32 NewIndex = ((Item.LocationInfo.Get<2>().Y + ItemY) * SizeX) + (Item.LocationInfo.Get<2>().X + ItemX);
					SlotWidgets[NewIndex]->bOccupied = false;
					SlotWidgets[NewIndex]->SetOverlayColor(FLinearColor::Transparent);
				}
			}
		}
	}
}

void UW_InventorySlotCollection::PopulateSlotWithItem(FVector2D InventorySlot, AItemBase* Item, bool bVertical)
{
	if (IsValid(Item) == false)
		return;
	if (IsValid(Item->ItemComponent->ItemStruct.ItemData) == false)
		return;

	if (UW_InventoryItem* InventoryItem = CreateWidget<UW_InventoryItem>(GetOwningPlayer(), InventoryItemWidget))
	{
		Item->ItemComponent->ItemStruct.InventoryItemWidgetRef = InventoryItem;

		InventoryItem->ItemStruct = Item->ItemComponent->ItemStruct;
		
		UE_LOG(LogTemp, Warning, TEXT("Create Inventory item widget") );
		if (auto ItemIcon = Item->ItemComponent->ItemStruct.ItemData->Icon)
		{
			InventoryItem->Icon->SetBrushFromTexture(ItemIcon);
		}


		if(Overlay)
		{
			auto OverlaySlot = Overlay->AddChildToOverlay(InventoryItem);
			OverlaySlot->SetHorizontalAlignment(HAlign_Left);
			OverlaySlot->SetVerticalAlignment(VAlign_Top);
			ForceLayoutPrepass();
			SlotWidgets[0]->ForceLayoutPrepass();
			FVector2D DesiredSize = GetDesiredSize();
			UE_LOG(LogTemp, Warning, TEXT("Desired Widget Size: %s"),  *DesiredSize.ToString());
			
			FVector2D DesiredLocation = (GetDesiredSize() / SlotWidgets[0]->GetDesiredSize()) * FVector2D{InventorySlot.X, InventorySlot.Y};
			UE_LOG(LogTemp, Warning, TEXT("Desired Item Widget Location: %s"),  *DesiredLocation.ToString());
			
			InventoryItem->SizeBox->SetHeightOverride(SlotWidgets[0]->GetDesiredSize().Y * Item->ItemComponent->ItemStruct.ItemData->SizeY);
			InventoryItem->SizeBox->SetWidthOverride(SlotWidgets[0]->GetDesiredSize().X * Item->ItemComponent->ItemStruct.ItemData->SizeX);
			FMargin NewPadding{};
			NewPadding.Top = (SlotWidgets[0]->GetDesiredSize().Y * Item->ItemComponent->ItemStruct.ItemData->SizeY) * InventorySlot.Y;
			NewPadding.Left = (SlotWidgets[0]->GetDesiredSize().X * Item->ItemComponent->ItemStruct.ItemData->SizeX) * InventorySlot.X;
			OverlaySlot->SetPadding(NewPadding);
			
			Overlay->InvalidateLayoutAndVolatility();
		}
				
	}
	
	if (bVertical)
	{
		for(int32 ItemY = 0; ItemY < Item->ItemComponent->ItemStruct.ItemData->SizeX; ItemY++)
		{
			for(int32 ItemX = 0; ItemX < Item->ItemComponent->ItemStruct.ItemData->SizeY; ItemX++)
			{
				int32 NewIndex = ((InventorySlot.Y + ItemY) * SizeX) + (InventorySlot.X + ItemX);
				SlotWidgets[NewIndex]->SetOverlayColor(Item->ItemComponent->ItemStruct.ItemData->BackgroundColor);
				SlotWidgets[NewIndex]->bOccupied = true;
			}
		}
	}
	else
	{
		for(int32 ItemY = 0; ItemY < Item->ItemComponent->ItemStruct.ItemData->SizeY; ItemY++)
		{
			for(int32 ItemX = 0; ItemX < Item->ItemComponent->ItemStruct.ItemData->SizeX; ItemX++)
			{
				int32 NewIndex = ((InventorySlot.Y + ItemY) * SizeX) + (InventorySlot.X + ItemX);
				SlotWidgets[NewIndex]->SetOverlayColor(Item->ItemComponent->ItemStruct.ItemData->BackgroundColor);
				SlotWidgets[NewIndex]->bOccupied = true;
			}
		}
	}
}
