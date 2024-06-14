#include "ItemComponent.h"

#include "TheGrowth/HUD/Widgets/Inventory/W_InventoryItem.h"
#include "TheGrowth/Items/ItemBase.h"

void UItemComponent::AddItem(AItemBase* Item, TTuple<UW_InventoryContainer*, int, FVector2D>& LocationInfo)
{
	if (IsValid(Item) == false)
		return;

	if (IsValid(Item->ItemComponent->ItemStruct.ItemData) == false)
		return;
	
	
	ItemStruct.Inventory.Add(Item->ItemComponent->ItemStruct);
}

void UItemComponent::AddItem(FItemStruct& Item, TTuple<UW_InventoryContainer*, int, FVector2D> LocationInfo)
{
	if (IsValid(Item.ItemData) == false)
		return;
	
	Item.LocationInfo = LocationInfo;
	if (ItemStruct.Inventory.Num() <ItemStruct.MaxInventorySize)
		ItemStruct.Inventory.Add(Item);
}

void UItemComponent::RemoveItem(FItemStruct& Item)
{
	if (IsValid(Item.InventoryItemWidgetRef))
	{
		Item.InventoryItemWidgetRef->RemoveFromParent();
		Item.InventoryItemWidgetRef->MarkAsGarbage();
		Item.InventoryItemWidgetRef = nullptr;
	}
	
	ItemStruct.Inventory.RemoveSingle(Item);
	ItemStruct.Inventory.Shrink();
}
