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

protected:
	UFUNCTION()
	void AddItem(class AItemBase* Item);

protected:
	UPROPERTY(EditAnywhere, Category= Inventory, meta = (AllowPrivateAccess = "true"))
	TArray<FItemStruct> Inventory{};

};
