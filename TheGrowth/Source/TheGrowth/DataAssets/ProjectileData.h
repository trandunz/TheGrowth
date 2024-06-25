#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProjectileData.generated.h"

UCLASS()
class THEGROWTH_API UProjectileData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 PenetrationRating{0};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 RicochetChance{0};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TravelSpeed{86600.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Damage{35};
};
