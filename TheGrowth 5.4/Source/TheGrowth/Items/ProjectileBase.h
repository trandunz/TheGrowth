#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

UCLASS()
class THEGROWTH_API AProjectileBase : public AActor
{
	friend class AWeaponBase;
	GENERATED_BODY()

	AProjectileBase();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	void SetDirectionOfMotion(FVector NewDirection);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool EnableDrawDebugLine{false};
	
	UPROPERTY(EditAnywhere)
	float TravelSpeed{20000.0f};
	float BulletTime{0.0f};
	UPROPERTY(BlueprintReadOnly)
	FRotator BulletAngle;
	UPROPERTY(Replicated)
	FVector Velocity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Gravity = -50.0f;
	float RotationPitch;
	UPROPERTY(EditDefaultsOnly)
	float BulletLifetime = 30.0f;

	UPROPERTY(EditAnywhere)
	int Damage{35};

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UArrowComponent* ArrowComponent{nullptr};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Mesh{nullptr};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UParticleSystemComponent* TracerParticleSystem{nullptr};
};
