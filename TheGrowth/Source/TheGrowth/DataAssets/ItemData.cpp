// Copyright William Inman. All Rights Reserved.

#include "ItemData.h"

bool FItemStruct::AddItemToInventory(FItemStruct& Item)
{
	bool bSuccess{false};

	if (Inventory.Num() < MaxInventorySize && IsValid(Item.ItemData))
	{
		bSuccess = true;
		Inventory.Add(Item);
	}
	
	return bSuccess;
}
