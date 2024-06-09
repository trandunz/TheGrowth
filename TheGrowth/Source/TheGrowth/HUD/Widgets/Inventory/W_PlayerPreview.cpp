#include "W_PlayerPreview.h"

#include "TheGrowth/Pawns/DummySurvivalCharacter.h"

FReply UW_PlayerPreview::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (IsValid(DummyCharacter) == false)
		Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	bRotateDummy = true;
	
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UW_PlayerPreview::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (IsValid(DummyCharacter) == false)
		Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	if (bRotateDummy)
	{
		DummyCharacter->RotateZ(InMouseEvent.GetCursorDelta().X);
	}
	
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

FReply UW_PlayerPreview::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (IsValid(DummyCharacter) == false)
		Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	bRotateDummy = false;
	
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}
