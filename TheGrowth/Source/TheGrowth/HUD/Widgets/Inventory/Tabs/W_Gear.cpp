#include "W_Gear.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "TheGrowth/Components/ItemComponent.h"
#include "TheGrowth/DataAssets/InventoryContainerData.h"
#include "TheGrowth/DataAssets/ItemData.h"
#include "TheGrowth/HUD/Widgets/Inventory/W_InventoryContainer.h"
#include "TheGrowth/HUD/Widgets/Inventory/W_InventoryItem.h"
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
		if (Container->SlotType == Item->ItemComponent->ItemStruct.ItemData->ItemType)
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

	if (IsValid(Item->ItemComponent->ItemStruct.ItemData) == false)
		return LocationInformation;

	for(const auto Container : Containers)
	{
		if (Container->SlotType == Item->ItemComponent->ItemStruct.ItemData->ItemType)
		{
			if (Container->bOccupied == false)
			{
				UE_LOG(LogTemp, Warning, TEXT("Equip item immediatly") );
				LocationInformation.Get<0>() = Container;
				return LocationInformation;
			}
		}

		TTuple<int, FVector2D> ContainerInformation = Container->PickupItem(Item);
		//UE_LOG(LogTemp, Warning, TEXT("Picked up item into %s at slot collectio") );
		if (ContainerInformation.Get<0>() != -1)
		{
			LocationInformation.Get<0>() = Container;
			LocationInformation.Get<1>() = ContainerInformation.Get<0>();
			LocationInformation.Get<2>() = ContainerInformation.Get<1>();
			UE_LOG(LogTemp, Warning, TEXT("Attempt to pickup item into inventory container %s at inventory collection %s at slot %s"), *LocationInformation.Get<0>()->GetName(), *FString::FromInt(LocationInformation.Get<1>()), *LocationInformation.Get<2>().ToString());
			Item->ItemComponent->ItemStruct.LocationInfo = LocationInformation;
			Item->ItemComponent->ItemStruct.PopulateInventorySlotWithItem();
			return LocationInformation;
		}
	}
	
	return LocationInformation;
}

void UW_Gear::RemoveItem(FItemStruct& Item)
{
	TTuple<UW_InventoryContainer*, int, FVector2D>& LocationInformation = Item.LocationInfo;
	
	Item.InventoryItemWidgetRef->RemoveFromParent();
	
	if (IsValid(LocationInformation.Get<0>()) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Container reference was invalid when removing item"));
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Attempt to remove item from inventory container %s at inventory collection %s at slot %s"), *LocationInformation.Get<0>()->GetName(), *FString::FromInt(LocationInformation.Get<1>()), *LocationInformation.Get<2>().ToString());
	LocationInformation.Get<0>()->RemoveItem(Item);
}

bool UW_Gear::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	//UE_LOG(LogTemp, Warning, TEXT("Drop Item Attempt") );

	//UW_InventoryItem* ItemWidget = Cast<UW_InventoryItem>(InOperation->Payload);
	//if (IsValid(ItemWidget) == false)
	//	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
//
	//FVector2D pixelPosition;
	//FVector2D viewportMinPosition;
	//FVector2D viewportMaxPosition;
//
	//// Get the top left and bottom right viewport positions
	//USlateBlueprintLibrary::LocalToViewport( GetWorld(), InGeometry, FVector2D( 0, 0 ), pixelPosition, viewportMinPosition );
	//USlateBlueprintLibrary::LocalToViewport( GetWorld(), InGeometry, InGeometry.GetLocalSize(), pixelPosition, viewportMaxPosition );
	//
	//FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport( GetWorld() );
	//FVector2D RelativePos = ( MousePos - viewportMinPosition ) / ( viewportMaxPosition - viewportMinPosition );
	
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
