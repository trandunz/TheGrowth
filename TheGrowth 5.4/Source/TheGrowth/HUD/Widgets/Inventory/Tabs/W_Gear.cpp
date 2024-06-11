#include "W_Gear.h"

#include "TheGrowth/DataAssets/InventoryContainerData.h"
#include "TheGrowth/DataAssets/ItemData.h"
#include "TheGrowth/HUD/Widgets/Inventory/W_InventoryContainer.h"
#include "TheGrowth/Items/ItemBase.h"

void UW_Gear::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Containers =
		{
		Earpiece,
		Head,
		Face,
		Arm,
		Body,
		EyeWear,
		Holster,
		Melee,
		Primary,
		Secondary,
		
		Rig,
		Pockets,
		Specials,
		Backpack,
		Pouch
		};
}

bool UW_Gear::CanPickupItem(AItemBase* Item)
{
	if (IsValid(Item) == false)
		return false;

	// Generic Item
	for(auto Container : Containers)
	{
		if (Container->SlotType == Item->ItemData->ItemType)
		{
			if (Container->bOccupied == false)
				return true;
		}

		if (Container->CanFitItem(Item))
			return true;
	}

	return false;
}
