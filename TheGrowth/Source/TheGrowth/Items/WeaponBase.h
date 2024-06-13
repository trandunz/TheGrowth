#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

#define CHAMBER_SLOT_ID "Chamber"
#define MAGAZINE_SLOT_ID "Magazine"

UCLASS()
class THEGROWTH_API AWeaponBase : public AItemBase
{
	GENERATED_BODY()
	friend class ASurvivalCharacter;
	
	AWeaponBase();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	UFUNCTION()
	bool CanFitMagazine(class UItemData* Magazine);
	
protected:
	virtual void Discharge();
	virtual void Reload(struct FItemStruct& Magazine);

	void LoadChamberFromMagazine();
	void LoadChamberFromMagazine(struct FItemStruct& Magazine);

	UFUNCTION()
	void DecrementFireRateTimer(float DeltaSeconds);
	UFUNCTION()
	void DecrementRecoilTimer(float DeltaSeconds);

	UFUNCTION()
	void PlayDischargeSound();
	UFUNCTION()
	void PlayReloadSound();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Firearm")
	float FireRateTimer{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Firearm")
	float RecoilTimer{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Firearm")
	int32 CurrentRecoilIndex{};
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Firearm")
	class UFirearmData* FirearmData{nullptr};
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	class UEquipmentComponent* Attachments{nullptr};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	class UParticleSystemComponent* MuzzleFlashVFX{nullptr};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	class UAudioComponent* AudioComponent{nullptr};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	class UAudioComponent* HandlingAudioComponent{nullptr};
	
};
