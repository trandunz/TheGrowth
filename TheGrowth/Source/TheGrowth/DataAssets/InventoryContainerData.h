#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InventoryContainerData.generated.h"

UCLASS()
class THEGROWTH_API UInventoryContainerData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UTexture2D* Icon{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> ContainerWidget{};
};
