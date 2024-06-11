#include "EquipmentComponent.h"

UEquipmentComponent::UEquipmentComponent()
{
}

FItemStruct* UEquipmentComponent::GetSlotByName(FString Name)
{
	if (Slots.Contains(Name))
	{
		return &Slots[Name];
	}

	return nullptr;
}

void UEquipmentComponent::EmptyNamedSlot(FString Name)
{
	if (Slots.Contains(Name))
	{
		Slots[Name] = {};
	}
}

void UEquipmentComponent::PopulateNamedSlot(FString Name, FItemStruct Item)
{
	if (Slots.Contains(Name))
	{
		Slots[Name] = Item;
	}
}
