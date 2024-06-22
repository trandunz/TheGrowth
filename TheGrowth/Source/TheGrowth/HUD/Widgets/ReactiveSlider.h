#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TimelineComponent.h"
#include "ReactiveSlider.generated.h"

UCLASS()
class THEGROWTH_API UReactiveSlider : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
public:
	UFUNCTION()
	void SetSliderPercentage(float Percentage, bool bInstant = false);

protected:
	UFUNCTION()
	void OnFadeTimelineUpdated(float Delta);
	UFUNCTION()
	void OnValueLerpFinished();
	UFUNCTION()
	void OnFadeTimelineFinished();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveFloat* FadeCurve{nullptr};
	
	UPROPERTY(VisibleAnywhere)
	FTimeline FadeTimeline{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTimerHandle FadeTimerHandle{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TimeToFade{0.3f};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TimeBeforeFadeReset{3.0f};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CachedPercentage{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float TargetPercentage{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CachedOpacity{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bReversingFade{};

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	class UProgressBar* ProgressBar{nullptr};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	class UBorder* Background{nullptr};
};
