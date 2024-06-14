// Copyright William Inman. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TheGrowth/Interfaces/InteractInterface.h"
#include "ItemBase.generated.h"

UCLASS()
class THEGROWTH_API AItemBase : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	friend class UItemComponent;
	friend class UInventoryComponent;
	friend class UW_InventorySlotCollection;
	friend class UW_InventoryContainer;
	friend class UW_Gear;

	
protected:
	AItemBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	virtual void Interact(class ASurvivalCharacter* Character) override;

public:
	UFUNCTION()
	void TogglePhysics(bool Enabled);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Components, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* MeshComponent{nullptr};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Components, meta = (AllowPrivateAccess = "true"))
	class UItemComponent* ItemComponent{nullptr};
};
