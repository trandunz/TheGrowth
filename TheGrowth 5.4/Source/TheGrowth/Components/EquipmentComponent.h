// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEGROWTH_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

	UEquipmentComponent();

public:
	UFUNCTION()
	class UItemComponent* GetSlotByName(FString Name);

	UFUNCTION()
	void EmptyNamedSlot(FString Name);
	
	UFUNCTION()
	void PopulateNamedSlot(FString Name, class UItemComponent* Item);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, class UItemComponent*> Slots{};
};
