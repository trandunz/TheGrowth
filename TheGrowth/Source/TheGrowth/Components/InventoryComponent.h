// Copyright William Inman. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

USTRUCT()
struct FItem
{
	GENERATED_BODY()

	class UItemData* ItemData{};
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEGROWTH_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
	friend class ASurvivalCharacter;
	friend class UW_Inventory;
	
protected:
	UInventoryComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UFUNCTION()
	void ChangeInventorySize(int32 NewX, int32 NewY);

	UFUNCTION()
	class UItemData* GetItemAtIndex(int32 X, int32 Y);

	UFUNCTION()
	void AddItem(class AItemBase* Item);

	UFUNCTION()
	bool CanFitItemAtIndex(class AItemBase* Item, int32 X, int32 Y);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Inventory, meta = (AllowPrivateAccess = "true"))
	int32 SizeX{10};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Inventory, meta = (AllowPrivateAccess = "true"))
	int32 SizeY{20};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Inventory, meta = (AllowPrivateAccess = "true"))
	TArray<class UItemData*> Inventory{};

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= References, meta = (AllowPrivateAccess = "true"))
	class UW_Inventory* InventoryWidget{};

};
