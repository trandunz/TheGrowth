#include "W_InventoryContainerLayout.h"
#include "W_InventorySlotCollection.h"

void UW_InventoryContainerLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsValid(SlotCollection))
		SlotCollections.Add(SlotCollection);
	if (IsValid(SlotCollection1))
		SlotCollections.Add(SlotCollection);
	if (IsValid(SlotCollection2))
		SlotCollections.Add(SlotCollection);
	if (IsValid(SlotCollection3))
		SlotCollections.Add(SlotCollection);
	if (IsValid(SlotCollection4))
    	SlotCollections.Add(SlotCollection);
	if (IsValid(SlotCollection5))
		SlotCollections.Add(SlotCollection);
}
