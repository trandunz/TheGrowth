#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_Overall.generated.h"

UCLASS()
class THEGROWTH_API UW_Overall : public UUserWidget
{
	friend class UW_Inventory;
	GENERATED_BODY()

protected:
	UFUNCTION()
	void SetPreviewDummyCharacterReference(class ADummySurvivalCharacter* DummyCharacter);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UW_PlayerPreview* PlayerPreview{};
};
