#include "ReactiveSlider.h"

#include "Components/Border.h"
#include "Components/ProgressBar.h"
#include "Components/Slider.h"

void UReactiveSlider::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsValid(FadeCurve))
	{
		FOnTimelineFloat ProgressFunction{};
		ProgressFunction.BindDynamic(this, &UReactiveSlider::OnFadeTimelineUpdated); // The function EffectProgress gets called
		FadeTimeline.AddInterpFloat(FadeCurve, ProgressFunction);
		FOnTimelineEvent TimelineFinishedFunction{};
		TimelineFinishedFunction.BindDynamic(this, &UReactiveSlider::OnFadeTimelineFinished);
		FadeTimeline.SetTimelineFinishedFunc(TimelineFinishedFunction);
	}
}

void UReactiveSlider::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FadeTimeline.TickTimeline(InDeltaTime);
}

void UReactiveSlider::SetSliderPercentage(float Percentage, bool bInstant)
{
	if (bInstant)
	{
		ProgressBar->SetPercent(Percentage);
		TargetPercentage = Percentage;
		CachedPercentage = Percentage;
		GetWorld()->GetTimerManager().SetTimer(FadeTimerHandle, this, &UReactiveSlider::OnValueLerpFinished, TimeBeforeFadeReset);
		return;
	}
	
	if (TargetPercentage == Percentage)
		return;
	
	TargetPercentage = Percentage;
	CachedOpacity = Background->GetRenderOpacity();

	FadeTimeline.SetPlayRate(1.0f / FMath::Clamp(TimeToFade, 0.001f, 10.0f));
	FadeTimeline.PlayFromStart();
}

void UReactiveSlider::OnFadeTimelineUpdated(float Delta)
{
	if (IsValid(ProgressBar) == false)
		return;
	if (TargetPercentage != CachedPercentage)
		ProgressBar->SetPercent(FMath::Lerp(CachedPercentage, TargetPercentage, Delta));

	if (IsValid(Background) == false)
    	return;
	ProgressBar->SetRenderOpacity(FMath::Lerp(CachedOpacity, 1.0f, Delta));
	Background->SetRenderOpacity(FMath::Lerp(CachedOpacity, 1.0f, Delta));
	
	UE_LOG(LogTemp, Warning, TEXT("Updated Health Bar Percentage %s"), *FString::SanitizeFloat(FMath::Lerp(CachedPercentage, TargetPercentage, Delta)));
}

void UReactiveSlider::OnValueLerpFinished()
{
	CachedOpacity = 0.0f;
	bReversingFade = true;
	FadeTimeline.ReverseFromEnd();
}

void UReactiveSlider::OnFadeTimelineFinished()
{
	if (bReversingFade)
	{
		bReversingFade = false;
		return;
	}
	
	CachedPercentage = TargetPercentage;

	GetWorld()->GetTimerManager().SetTimer(FadeTimerHandle, this, &UReactiveSlider::OnValueLerpFinished, TimeBeforeFadeReset);
}
