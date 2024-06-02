#include "W_InventoryContainer.h"

#include "Components/NamedSlot.h"
#include "TheGrowth/DataAssets/InventoryContainerData.h"

void UW_InventoryContainer::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(ContainerWidget) == false)
		return;
	
	if (IsValid(ContainerData) == false)
	{
		ContainerWidget->ClearChildren();
		ContainerWidget->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
		

	bool ShouldCreateContainerWidget{true};
	if (auto ContentSlot = ContainerWidget->GetContentSlot())
	{
		if (auto Content = ContentSlot->Content)
		{
			if (Content->GetClass() == ContainerData->ContainerWidget)
			{
				ShouldCreateContainerWidget = false;
			}
		}
	}

	if (ShouldCreateContainerWidget)
	{
		auto NewWidget = CreateWidget(ContainerWidget, ContainerData->ContainerWidget);
		ContainerWidget->ClearChildren();
		ContainerWidget->AddChild(NewWidget);
		ContainerWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
