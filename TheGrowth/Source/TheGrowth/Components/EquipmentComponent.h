// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TheGrowth/DataAssets/ItemData.h"
#include "EquipmentComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEGROWTH_API UEquipmentComponent : public UActorComponent
{
	friend class AWeaponBase;
	GENERATED_BODY()

	UEquipmentComponent();

public:
	FItemStruct* GetSlotByName(FString Name);

	UFUNCTION()
	void EmptyNamedSlot(FString Name);
	
	UFUNCTION()
	void PopulateNamedSlot(FString Name, struct FItemStruct Item);

protected:
	UPROPERTY(EditAnywhere)
	TMap<FString, FItemStruct> Slots{};
};
