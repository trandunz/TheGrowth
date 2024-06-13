// Copyright William Inman. All Rights Reserved.

#include "InventoryComponent.h"

#include "ItemComponent.h"
#include "TheGrowth/Items/ItemBase.h"

void UInventoryComponent::AddItem(AItemBase* Item, TTuple<UW_InventoryContainer*, int, FVector2D>& LocationInfo)
{
	if (IsValid(Item) == false)
		return;

	if (IsValid(Item->ItemComponent->ItemData) == false)
		return;
	
	
	Inventory.Add({Item->ItemComponent->ItemData, Item->ItemComponent->Inventory, LocationInfo, Item->ItemComponent->bRotated});
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
	Inventory.RemoveSingle(Item);
	Inventory.Shrink();
}
