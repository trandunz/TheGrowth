#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_PlayerPreview.generated.h"

UCLASS()
class THEGROWTH_API UW_PlayerPreview : public UUserWidget
{
	friend class UW_Overall;
	GENERATED_BODY()

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class ADummySurvivalCharacter* DummyCharacter{nullptr};

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bRotateDummy{};
};
