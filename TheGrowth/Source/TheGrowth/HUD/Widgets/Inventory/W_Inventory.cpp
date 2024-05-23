// Copyright William Inman. All Rights Reserved.

#include "W_Inventory.h"

#include "W_InventoryGrid.h"
#include "W_InventoryItem.h"
#include "W_InventorySlot.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/UniformGridPanel.h"
#include "TheGrowth/Components/InventoryComponent.h"
#include "TheGrowth/DataAssets/ItemData.h"

UW_InventorySlot* UW_Inventory::GetGridSlotAtIndex(int32 X, int32 Y)
{
	return InventoryGrid->GetSlotAtIndex(X, Y);
}

void UW_Inventory::SetGridSlotColor(int32 X, int32 Y, FLinearColor Color)
{
	UW_InventorySlot* GridSlot = InventoryGrid->GetSlotAtIndex(X, Y);
	
	if (IsValid(GridSlot))
		GridSlot->SetOverlayColor(Color);
}

void UW_Inventory::AddItem(int32 X, int32 Y, UItemData* Item)
{
	if (IsValid(Item) == false)
		return;
	
	SetGridSlotColor(X, Y, Item->BackgroundColor);
	
	for(int32 NearbyY = Y; NearbyY < Y + Item->SizeY - 1; NearbyY++)
	{
		for(int32 NearbyX = X; NearbyX < X + Item->SizeX - 1; NearbyX++)
		{
			SetGridSlotColor(NearbyX, NearbyY, Item->BackgroundColor);
		}
	}
	
	if (IsValid(ItemWidgetClass) == false)
		return;
	
	UW_InventoryItem* ItemIcon = CreateWidget<UW_InventoryItem>(InventoryGrid->Overlay, ItemWidgetClass);
	ItemIcon->Icon->SetBrushFromTexture(Item->Icon, true);
	InventoryGrid->Overlay->AddChildToOverlay(ItemIcon);
	
	FVector2D SlotSize = InventoryGrid->GetSlotAtIndex(X, Y)->GetCachedGeometry().GetLocalSize();
	SlotSize.X *= Item->SizeX;
	SlotSize.Y *= Item->SizeY;
	ItemIcon->Icon->SetDesiredSizeOverride(SlotSize);
	UE_LOG(LogTemp, Warning, TEXT("Slot Size %s"), *SlotSize.ToString() );
	
	FVector2D ViewportPos;
	FVector2D PixelPos;
	USlateBlueprintLibrary::LocalToViewport(GetWorld(), InventoryGrid->GetSlotAtIndex(X, Y)->GetCachedGeometry(), {0.0f, 0.0f}, PixelPos, ViewportPos);
	ItemIcon->SetPositionInViewport(ViewportPos);
	
	UE_LOG(LogTemp, Warning, TEXT("Item Icon Spawned at %s"), *ViewportPos.ToString() );
	
	
	ItemIcons.Add({(double)X, (double)Y}, ItemIcon);
}
