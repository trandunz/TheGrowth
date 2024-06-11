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

protected:
	virtual void Discharge();
	virtual void Reload();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UEquipmentComponent* Attachments{nullptr};
	
};
