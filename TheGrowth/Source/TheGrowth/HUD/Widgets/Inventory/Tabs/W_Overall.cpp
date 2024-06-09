#include "W_Overall.h"

#include "TheGrowth/HUD/Widgets/Inventory/W_PlayerPreview.h"

void UW_Overall::SetPreviewDummyCharacterReference(ADummySurvivalCharacter* DummyCharacter)
{
	PlayerPreview->DummyCharacter = DummyCharacter;
}
