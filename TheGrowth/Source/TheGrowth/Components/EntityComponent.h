// Copyright William Inman. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EntityComponent.generated.h"

UENUM()
enum EBodyPart
{
	HEAD,
	CHEST,
	STOMACH,
	ARM_L,
	ARM_R,
	LEG_L,
	LEG_R
};

USTRUCT(BlueprintType)
struct FLimbStatus
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health{100.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bBleeding{false};
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEGROWTH_API UEntityComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UEntityComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	static EBodyPart ConvertBoneToBodyPart(FString Bone);
	
	UFUNCTION()
	void OffsetHealth(EBodyPart BodyPart, float Amount, bool ApplyBleeding = true);

	UFUNCTION()
	void BleedOut(float DeltaTime);

	UFUNCTION()
	void OffsetStamina(float Amount);
	
	UFUNCTION()
	bool IsDead() const;

	UFUNCTION()
	void BeginStaminaReset();
	UFUNCTION()
	void RegenerateStamina(float DeltaSeconds);

public: // Setters & Getters //
	float GetTotalCurrentHealth() const;
	float GetTotalMaxHealth() const;

	FORCEINLINE float GetCurrentStamina() const {return CurrentStamina;}
	FORCEINLINE float GetMaxStamina() const {return MaxStamina;}

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= References)
	class ACharacterBase* OwningCharacter{nullptr};
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Entity)
	TMap<TEnumAsByte<EBodyPart>, FLimbStatus> CurrentBodyPartHealth{
		{EBodyPart::HEAD, {35.0f}},
		{EBodyPart::CHEST, {85.0f}},
		{EBodyPart::STOMACH, {70.0f}},
		{EBodyPart::LEG_L, {65.0f}},
		{EBodyPart::LEG_R, {65.0f}},
		{EBodyPart::ARM_L, {60.0f}},
		{EBodyPart::ARM_R, {60.0f}},
	};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Entity)
	TMap<TEnumAsByte<EBodyPart>, FLimbStatus> MaxBodyPartHealth{
			{EBodyPart::HEAD, {35.0f}},
			{EBodyPart::CHEST, {85.0f}},
			{EBodyPart::STOMACH, {70.0f}},
			{EBodyPart::LEG_L, {65.0f}},
			{EBodyPart::LEG_R, {65.0f}},
			{EBodyPart::ARM_L, {60.0f}},
			{EBodyPart::ARM_R, {60.0f}},
		};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Entity)
	float BloodVolume{5000};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Entity)
	float MinimumBloodVolume{3000};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Entity)
	float ArteryPunctureVolumeRate{100};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Entity)
	float FleshWoundDamageRate{15};
	
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

