#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ArmourData.generated.h"

/*UENUM()
enum EProtectionArea
{
	HEAD_TOP,
	HEAD_EYES,
	HEAD_EARS,
	HEAD_CHIN,

	NECK,

	CHEST,
	STOMACH,
};*/

UCLASS()
class THEGROWTH_API UArmourData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ArmourRating{0};
	
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//EProtectionArea ProtectionAreas{};
};
