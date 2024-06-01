// Copyright William Inman. All Rights Reserved.

#include "W_Inventory.h"

#include "Components/WidgetSwitcher.h"

void UW_Inventory::SetSelectedWidgetIndex(int32 Index)
{
	MenuSelector->SetActiveWidgetIndex(Index);
}
