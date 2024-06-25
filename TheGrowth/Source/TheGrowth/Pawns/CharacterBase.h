#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS()
class THEGROWTH_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

protected:
	ACharacterBase();
	
public:
	virtual void SetRagdoll(bool Ragdoll) const;
	
public: // Entity Component
	UFUNCTION()
	FORCEINLINE class UEntityComponent* GetEntityComponent() const {return EntityComponent;}
	
	virtual void TakeDamage(FString Bone, class AProjectileBase* Projectile);
	virtual void TakeDamage(FString Bone, float Amount);
	virtual void TakeDamage(float Amount);
	
	virtual void OffsetStamina(float Amount);
	virtual bool IsDead() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Components)
	class UEntityComponent* EntityComponent{nullptr};
};
