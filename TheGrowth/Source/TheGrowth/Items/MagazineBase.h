#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "TheGrowth/DataAssets/ItemData.h"
#include "MagazineBase.generated.h"

UCLASS()
class THEGROWTH_API AMagazineBase : public AItemBase
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	bool LoadRound(FItemStruct Round);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Magazine")
	TArray<UItemData*> CompatibleRounds{};
};
