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
	UPROPERTY(EditAnywhere)
	float TravelSpeed{86600.0f};
	float BulletTime{0.0f};
	UPROPERTY(Replicated)
	FVector Velocity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Gravity = -981.0f;
	float RotationPitch;
	UPROPERTY(EditDefaultsOnly)
	float BulletLifetime = 30.0f;

	UPROPERTY(EditAnywhere)
	int Damage{35};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDrawDebugTracer{true};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DebugTracerWidth{0.5f};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DebugTracerLifetime{1.0f};

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> ActorsToIngore{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UParticleSystem* BulletImpactVFX{nullptr};

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UArrowComponent* ArrowComponent{nullptr};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Mesh{nullptr};
};
