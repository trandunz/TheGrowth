#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_InventoryContainerLayout.generated.h"

UCLASS()
class THEGROWTH_API UW_InventoryContainerLayout : public UUserWidget
{
	GENERATED_BODY()
	friend class UW_InventoryContainer;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<class UW_InventorySlotCollection*> SlotCollections{};
};
