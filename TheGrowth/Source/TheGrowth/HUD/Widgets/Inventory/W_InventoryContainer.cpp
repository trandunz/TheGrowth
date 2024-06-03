#include "W_InventoryContainer.h"

#include "W_InventoryContainerLayout.h"
#include "W_InventorySlot.h"
#include "W_InventorySlotCollection.h"
#include "Components/Image.h"
#include "Components/NamedSlot.h"
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
	
	if (Item->ItemData->SizeY * Item->ItemData->SizeX > ContainerData->TotalInventorySize)
		return false;

	bool CanFitItem{true};
	for(auto SlotCollection : LayoutWidget->SlotCollections)
	{
		if (SlotCollection->CanFitItem(Item) == false)
			CanFitItem = false;
	}

	return CanFitItem;
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
		
		auto NewWidget = CreateWidget(ContainerWidget, ContainerData->ContainerLayout);

		LayoutWidget = Cast<UW_InventoryContainerLayout>(NewWidget);
		
		ContainerWidget->ClearChildren();
		ContainerWidget->AddChild(NewWidget);
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
