// Copyright William Inman. All Rights Reserved.

#include "InventoryComponent.h"

#include "ItemComponent.h"
#include "TheGrowth/HUD/Widgets/Inventory/W_InventoryItem.h"
#include "TheGrowth/Items/ItemBase.h"

void UInventoryComponent::AddItem(AItemBase* Item, TTuple<UW_InventoryContainer*, int, FVector2D>& LocationInfo)
{
	if (IsValid(Item) == false)
		return;

	if (IsValid(Item->ItemComponent->ItemStruct.ItemData) == false)
		return;
	
	
	Inventory.Add(Item->ItemComponent->ItemStruct);
}

void UInventoryComponent::AddItem(FItemStruct& Item, TTuple<UW_InventoryContainer*, int, FVector2D> LocationInfo)
{
	if (IsValid(Item.ItemData) == false)
		return;
	
	Item.LocationInfo = LocationInfo;
	if (Inventory.Num() < MaxInventorySize)
		Inventory.Add(Item);
}

void UInventoryComponent::RemoveItem(FItemStruct& Item)
{
	//if (IsValid(Item.InventoryItemWidgetRef))
	//{
	//	Item.InventoryItemWidgetRef->RemoveFromParent();
	//	Item.InventoryItemWidgetRef->MarkAsGarbage();
	//	Item.InventoryItemWidgetRef = nullptr;
	//}
	
	Inventory.RemoveSingle(Item);
	Inventory.Shrink();
}
