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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Configuration)
	float RecoilResetTime{0.5f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Configuration)
	TArray<FVector2D> RecoilTable{
		{0.000000,0.00000},
		{0.000000,-2.257792},
		{0.323242,-2.300758},
		{0.649593,-2.299759},
		{0.848786,-2.259034},
		{1.075408,-2.323947},
		{1.268491,-2.215956},
		{1.330963,-2.236556},
		{1.336833,-2.218203},
		{1.505516,-2.143454},
		{1.504423,-2.233091},
		{1.442116,-2.270194},
		{1.478543,-2.204318},
		{1.392874,-2.165817},
		{1.480824,-2.177887},
		{1.597069,-2.270915},
		{1.449996,-2.145893},
		{1.369179,-2.270450},
		{1.582363,-2.298334},
		{1.516872,-2.235066},
		{1.498249,-2.238401},
		{1.465769,-2.331642},
		{1.564812,-2.242621},
		{1.517519,-2.303052},
		{1.422433,-2.211946},
		{1.553195,-2.248043},
		{1.510463,-2.285327},
		{1.553878,-2.240047},
		{1.520380,-2.221839},
		{1.553878,-2.240047},
		{1.553195,-2.248043}
	};
};
