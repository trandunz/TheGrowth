#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class THEGROWTH_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	friend class ASurvivalCharacter;
	
	AWeaponBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* MeshComponent{};
};
