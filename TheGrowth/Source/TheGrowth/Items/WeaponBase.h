#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class THEGROWTH_API AWeaponBase : public AItemBase
{
	GENERATED_BODY()
	friend class ASurvivalCharacter;
	
	AWeaponBase();

public:
	UFUNCTION()
	bool CanFitMagazine(class UItemData* Magazine);
	
protected:
	virtual void Discharge();
	virtual void Reload(struct FItemStruct& Magazine);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Firearm")
	TArray<class UItemData*> MagazineTypes{};
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UEquipmentComponent* Attachments{nullptr};
};
