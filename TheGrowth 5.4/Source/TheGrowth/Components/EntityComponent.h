// Copyright William Inman. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EntityComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEGROWTH_API UEntityComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	UFUNCTION()
	void OffsetHealth(float Amount);
	UFUNCTION()
	bool IsDead() const;

public: // Setters & Getters //
	FORCEINLINE float GetCurrentHealth() const {return CurrentHealth;}
	FORCEINLINE float GetMaxHealth() const {return MaxHealth;}
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Entity)
	float CurrentHealth{100};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Entity)
	float MaxHealth{100};
};

