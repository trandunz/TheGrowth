// Copyright William Inman. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TheGrowth/DataAssets/ItemData.h"
#include "InventoryComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEGROWTH_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
	friend class ASurvivalCharacter;
	friend class UW_Inventory;
	friend class AWeaponBase;
	friend class AMagazineBase;
	friend class UW_InventorySlotCollection;

protected:
	
	void AddItem(class AItemBase* Item, TTuple<UW_InventoryContainer*, int, FVector2D>& LocationInfo);
	void AddItem(FItemStruct& Item, TTuple<UW_InventoryContainer*, int, FVector2D> LocationInfo = {});

	void RemoveItem(FItemStruct& Item);

	
protected:
	UPROPERTY(EditAnywhere, Category= Inventory, meta = (AllowPrivateAccess = "true"))
	TArray<FItemStruct> Inventory{};

	UPROPERTY(EditAnywhere, Category= Inventory, meta = (AllowPrivateAccess = "true"))
	int32 MaxInventorySize{9999};

};
