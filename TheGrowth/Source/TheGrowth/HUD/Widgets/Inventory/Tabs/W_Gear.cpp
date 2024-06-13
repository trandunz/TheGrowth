#include "W_Gear.h"

#include "TheGrowth/Components/ItemComponent.h"
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
		if (Container->SlotType == Item->ItemComponent->ItemData->ItemType)
		{
			if (Container->bOccupied == false)
				return true;
		}

		if (Container->CanFitItem(Item))
			return true;
	}

	return false;
}

TTuple<UW_InventoryContainer*, int, FVector2D> UW_Gear::PickupItem(AItemBase* Item)
{
	TTuple<UW_InventoryContainer*, int, FVector2D> LocationInformation(nullptr, -1, FVector2D{-1,-1});

	if (IsValid(Item) == false)
		return LocationInformation;

	if (IsValid(Item->ItemComponent->ItemData) == false)
		return LocationInformation;

	for(auto Container : Containers)
	{
		if (Container->SlotType == Item->ItemComponent->ItemData->ItemType)
		{
			if (Container->bOccupied == false)
			{
				UE_LOG(LogTemp, Warning, TEXT("Equip item immediatly") );
				LocationInformation.Get<0>() = Container;
				return LocationInformation;
			}
		}

		TTuple<int, FVector2D> ContainerInformation = Container->PickupItem(Item);
		if (ContainerInformation.Get<0>() != -1)
		{
			UE_LOG(LogTemp, Warning, TEXT("Add Item to inventory container") );
			LocationInformation.Get<0>() = Container;
			LocationInformation.Get<1>() = ContainerInformation.Get<0>();
			LocationInformation.Get<2>() = ContainerInformation.Get<1>();
			return LocationInformation;
		}
	}
	
	return LocationInformation;
}

void UW_Gear::RemoveItem(FItemStruct& Item)
{
	TTuple<UW_InventoryContainer*, int, FVector2D>& LocationInformation = Item.LocationInfo;
	LocationInformation.Get<0>()->RemoveItem(Item);
}
