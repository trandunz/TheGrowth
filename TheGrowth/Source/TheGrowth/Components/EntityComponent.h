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
	UEntityComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	UFUNCTION()
	void OffsetHealth(float Amount);

	UFUNCTION()
	void OffsetStamina(float Amount);
	
	UFUNCTION()
	bool IsDead() const;

	UFUNCTION()
	void BeginStaminaReset();
	UFUNCTION()
	void RegenerateStamina(float DeltaSeconds);

public: // Setters & Getters //
	FORCEINLINE float GetCurrentHealth() const {return CurrentHealth;}
	FORCEINLINE float GetMaxHealth() const {return MaxHealth;}

	FORCEINLINE float GetCurrentStamina() const {return CurrentStamina;}
	FORCEINLINE float GetMaxStamina() const {return MaxStamina;}
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Entity)
	float CurrentHealth{100};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Entity)
	float MaxHealth{100};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Entity)
	float CurrentStamina{100};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Entity)
	float MaxStamina{100};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Entity)
    float StaminaRegenerationSpeed{10.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Entity)
	float StaminaRegenerationDelay{2.0f};
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Entity)
	bool bShouldRegenStamina{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Entity)
	FTimerHandle StaminaRegenTimerHandle{};
};

