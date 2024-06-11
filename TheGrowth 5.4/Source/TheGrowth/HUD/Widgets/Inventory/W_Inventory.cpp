// Copyright William Inman. All Rights Reserved.

#include "W_Inventory.h"

#include "Components/WidgetSwitcher.h"
#include "Tabs/W_Overall.h"

void UW_Inventory::SetSelectedWidgetIndex(int32 Index)
{
	MenuSelector->SetActiveWidgetIndex(Index);
}

void UW_Inventory::SetPreviewDummyCharacterReference(ADummySurvivalCharacter* DummyCharacter)
{
	Overall->SetPreviewDummyCharacterReference(DummyCharacter);
}
