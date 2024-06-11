#include "MagazineBase.h"

#include "TheGrowth/Components/ItemComponent.h"

bool AMagazineBase::LoadRound(FItemStruct Round)
{
	bool bSuccess{false};
	for(const UItemData* CompatibleRound : CompatibleRounds)
	{
		if (Round.ItemData == CompatibleRound)
		{
			bSuccess = true;
			
			ItemComponent->AddItem(Round);
			
			break;
		}
	}
	
	return bSuccess;
}