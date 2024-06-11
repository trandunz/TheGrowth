#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FirearmData.generated.h"

UCLASS()
class THEGROWTH_API UFirearmData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Compatibility)
	TArray<class UItemData*> CompatibleMagazines{};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Specifications)
	float RPM{700.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Specifications)
	float Weight{2.92f};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Audio)
	class USoundCue* DischargeCue{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Audio)
	class USoundCue* DischargeCompleteCue{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Audio)
	class USoundAttenuation* DischargeAttenuation{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Audio)
	class USoundCue* ReloadCue{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Audio)
	class USoundAttenuation* ReloadAttenuation{};
};
