#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_InventoryItem.generated.h"

UCLASS()
class THEGROWTH_API UW_InventoryItem : public UUserWidget
{
	GENERATED_BODY()
	friend class UW_Inventory;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Components", meta = (BindWidget))
	class UImage* Icon{};
};
