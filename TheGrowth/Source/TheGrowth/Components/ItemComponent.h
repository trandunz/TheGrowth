#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.h"
#include "ItemComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEGROWTH_API UItemComponent : public UActorComponent
{
	GENERATED_BODY()
	friend class UW_Gear;
	friend class UInventoryComponent;
	friend class UW_InventorySlotCollection;
	friend class UW_InventoryContainer;
	
public:
	void AddItem(class AItemBase* Item, TTuple<UW_InventoryContainer*, int, FVector2D>& LocationInfo);
	void AddItem(FItemStruct& Item, TTuple<UW_InventoryContainer*, int, FVector2D> LocationInfo = {});

	void RemoveItem(FItemStruct& Item);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemStruct ItemStruct{};
};
