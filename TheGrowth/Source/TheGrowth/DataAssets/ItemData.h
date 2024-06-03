// Copyright William Inman. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"

UENUM()
enum EItemType
{
	Generic,
	
	Melee,
	Firearm,
	Sidearm,
	
	Earpiece,
	Head,
	Face,
	Arm,
	Body,
	EyeWear,
	
	Rig,
	BackPack,
	Pouch,
};

UCLASS()
class THEGROWTH_API UItemData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EItemType> ItemType{Generic};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UTexture2D* Icon{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor BackgroundColor{FColor::Cyan};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 SizeX{1};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 SizeY{1};
};
