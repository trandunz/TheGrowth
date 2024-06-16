#include "W_InventoryContainer.h"

#include "W_InventoryContainerLayout.h"
#include "W_InventorySlot.h"
#include "W_InventorySlotCollection.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/NamedSlot.h"
#include "TheGrowth/Components/ItemComponent.h"
#include "TheGrowth/DataAssets/InventoryContainerData.h"
#include "TheGrowth/DataAssets/ItemData.h"
#include "TheGrowth/Items/ItemBase.h"

void UW_InventoryContainer::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(ContainerWidget) == false)
		return;
	
	if (IsValid(ContainerData) == false)
	{
		bOccupied = false;
		
		ContainerWidget->ClearChildren();
		ContainerWidget->SetVisibility(ESlateVisibility::Collapsed);
		
		if (IsValid(ItemSlot))
		{
			if (ItemSlot->SlotWidgets.Num() > 0)
			{
				ItemSlot->SlotWidgets[0]->Icon->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		return;
	}
	
	UpdateContainer();
}

void UW_InventoryContainer::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

bool UW_InventoryContainer::IsOccupied() const
{
	return bOccupied;
}

bool UW_InventoryContainer::CanFitItem(AItemBase* Item)
{
	if (IsValid(ContainerData) == false)
		return false;

	if (IsValid(Item) == false)
		return false;

	if (IsValid(LayoutWidget) == false)
		return false;
	
	if (Item->ItemComponent->ItemStruct.ItemData->SizeY * Item->ItemComponent->ItemStruct.ItemData->SizeX > ContainerData->TotalInventorySize)
		return false;

	bool CanFitItem{false};
	for(auto SlotCollection : LayoutWidget->SlotCollections)
	{
		//if (SlotCollection->CanFitItem(Item))
		//{
		//	CanFitItem = true;
		//}
	}

	return CanFitItem;
}

TTuple<int, FVector2D> UW_InventoryContainer::PickupItem(AItemBase* Item)
{
	TTuple<int, FVector2D> LocationInformation(-1, FVector2D{-1,-1});

	if (IsValid(ContainerData) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Container Data Invalid") );
		return LocationInformation;
	}
	

	if (IsValid(Item) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item Reference Invalid") );
		return LocationInformation;
	}
		

	if (IsValid(LayoutWidget) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Layout Widget Invalid") );
		return LocationInformation;
	}
		
	
	if (Item->ItemComponent->ItemStruct.ItemData->SizeY * Item->ItemComponent->ItemStruct.ItemData->SizeX > ContainerData->TotalInventorySize)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item Too Large") );
		return LocationInformation;
	}

	
	for(int I = 0; I < LayoutWidget->SlotCollections.Num(); I++)
	{
		if (IsValid(LayoutWidget->SlotCollections[I]) == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("Slot Collection Invalid") );
			continue;
		}
		
		FVector2D SlotCollectionInformation = LayoutWidget->SlotCollections[I]->PickupItem(Item);
		if (SlotCollectionInformation.X != -1)
		{
			UE_LOG(LogTemp, Warning, TEXT("Add Item To Inventory Slot Collection") );
			LocationInformation.Get<0>() = I;
			LocationInformation.Get<1>() = SlotCollectionInformation;
			return LocationInformation;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Item Too Large For Container") );
	return LocationInformation;
}

void UW_InventoryContainer::RemoveItem(FItemStruct& Item)
{
	if (IsValid(LayoutWidget) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s container layout invalid"), *GetName());
		return;
	}

	if (Item.LocationInfo.Get<1>() != -1)
	{
		if (LayoutWidget->SlotCollections.Num() > Item.LocationInfo.Get<1>())
			LayoutWidget->SlotCollections[Item.LocationInfo.Get<1>()]->RemoveItem(Item);
	}
	else
		bOccupied = false;
}

void UW_InventoryContainer::UpdateContainer()
{
	bool ShouldCreateContainerWidget{true};
	if (auto ContentSlot = ContainerWidget->GetContentSlot())
	{
		if (auto Content = ContentSlot->Content)
		{
			if (Content->GetClass() == ContainerData->ContainerLayout)
			{
				ShouldCreateContainerWidget = false;
			}
		}
	}

	if (ShouldCreateContainerWidget)
	{
		bOccupied = true;

		LayoutWidget = WidgetTree->ConstructWidget(ContainerData->ContainerLayout);
		
		ContainerWidget->ClearChildren();
		ContainerWidget->AddChild(LayoutWidget);
		ContainerWidget->SetVisibility(ESlateVisibility::Visible);

		if (IsValid(ItemSlot))
		{
			if (ItemSlot->SlotWidgets.Num() > 0)
			{
				if (IsValid(ContainerData->Icon))
				{
					ItemSlot->SlotWidgets[0]->Icon->SetBrushFromTexture(ContainerData->Icon);
					ItemSlot->SlotWidgets[0]->Icon->SetVisibility(ESlateVisibility::Visible);
				}
			}
		}
	}
}
