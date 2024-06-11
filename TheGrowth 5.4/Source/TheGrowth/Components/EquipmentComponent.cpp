#include "EquipmentComponent.h"

UEquipmentComponent::UEquipmentComponent()
{
}

UItemComponent* UEquipmentComponent::GetSlotByName(FString Name)
{
	if (Slots.Contains(Name))
	{
		return Slots[Name];
	}

	return nullptr;
}

void UEquipmentComponent::EmptyNamedSlot(FString Name)
{
	if (Slots.Contains(Name))
	{
		Slots[Name] = nullptr;
	}
}

void UEquipmentComponent::PopulateNamedSlot(FString Name, UItemComponent* Item)
{
	if (Slots.Contains(Name))
	{
		Slots[Name] = Item;
	}
}
