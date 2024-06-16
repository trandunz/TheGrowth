// Copyright William Inman. All Rights Reserved.

#include "ItemData.h"

#include "TheGrowth/HUD/Widgets/Inventory/W_InventoryContainer.h"
#include "TheGrowth/HUD/Widgets/Inventory/W_InventoryContainerLayout.h"
#include "TheGrowth/HUD/Widgets/Inventory/W_InventorySlotCollection.h"

void FItemStruct::PopulateInventorySlotWithItem()
{
	if (IsValid(LocationInfo.Get<0>()) == false)
		return;

	if (LocationInfo.Get<1>() == -1)
		return;

	if (LocationInfo.Get<2>().X == -1)
		return;

	LocationInfo.Get<0>()->LayoutWidget->SlotCollections[LocationInfo.Get<1>()]->PopulateSlotWithItem(*this);
}
