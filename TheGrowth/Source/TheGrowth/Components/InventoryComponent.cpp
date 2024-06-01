// Copyright William Inman. All Rights Reserved.

#include "InventoryComponent.h"

#include "TheGrowth/DataAssets/ItemData.h"
#include "TheGrowth/HUD/Widgets/Inventory/W_Inventory.h"
#include "TheGrowth/HUD/Widgets/Inventory/W_InventorySlot.h"
#include "TheGrowth/Items/ItemBase.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ChangeInventorySize(SizeX, SizeY);
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryComponent::ChangeInventorySize(int32 NewX, int32 NewY)
{
	SizeX = NewX;
	SizeY = NewY;
	Inventory.SetNum(NewX * NewY);
}

UItemData* UInventoryComponent::GetItemAtIndex(int32 X, int32 Y)
{
	if (SizeX <= X)
		return nullptr;

	if (SizeY <= Y)
		return nullptr;
	
	return Inventory[(Y * SizeX) + X];
}

void UInventoryComponent::AddItem(AItemBase* Item)
{
	if (IsValid(Item) == false)
		return;

	if (IsValid(Item->ItemData) == false)
		return;

	for(int32 Y = 0; Y < SizeY; Y++)
	{
		for(int32 X = 0; X < SizeX; X++)
		{
			if (CanFitItemAtIndex(Item, X, Y))
			{
				Inventory[(Y * SizeX) + X] = Item->ItemData;
				
				for(int32 NearbyY = Y; NearbyY < Y + Item->ItemData->SizeY - 1; NearbyY++)
				{
					for(int32 NearbyX = X; NearbyX < X + Item->ItemData->SizeX - 1; NearbyX++)
					{
						Inventory[(NearbyY * SizeX) + NearbyX] = Item->ItemData;
					}
				}

				return;
			}
		}
	}
}

bool UInventoryComponent::CanFitItemAtIndex(AItemBase* Item, int32 X, int32 Y)
{
	if (GetItemAtIndex(X,Y))
		return false;
	
	for(int32 NearbyY = Y; NearbyY < Y + Item->ItemData->SizeY - 1; NearbyY++)
	{
		for(int32 NearbyX = X; NearbyX < X + Item->ItemData->SizeX - 1; NearbyX++)
		{
			if (GetItemAtIndex(NearbyX,NearbyY))
				return false;
			
			if (NearbyX >= SizeX)
				return false;
			
			if (NearbyY >= SizeY)
				return false;
		}
	}
	
	return true;
}

