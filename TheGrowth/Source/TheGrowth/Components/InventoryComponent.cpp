// Copyright William Inman. All Rights Reserved.

#include "InventoryComponent.h"

#include "ItemComponent.h"
#include "TheGrowth/Items/ItemBase.h"

void UInventoryComponent::AddItem(AItemBase* Item)
{
	if (IsValid(Item) == false)
		return;

	if (IsValid(Item->ItemComponent->ItemData) == false)
		return;
	
	Inventory.Add({Item->ItemComponent->ItemData, Item->ItemComponent->Inventory});
}

void UInventoryComponent::AddItem(FItemStruct& Item)
{
	if (IsValid(Item.ItemData) == false)
		return;

	if (Inventory.Num() < MaxInventorySize)
		Inventory.Add(Item);
}
