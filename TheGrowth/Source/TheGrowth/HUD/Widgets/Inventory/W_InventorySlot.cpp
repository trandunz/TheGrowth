// Copyright William Inman. All Rights Reserved.

#include "W_InventorySlot.h"
#include "Components/Image.h"

void UW_InventorySlot::SetOverlayColor(FLinearColor Color)
{
	if (Color == FColor::Transparent)
	{
		Overlay->SetColorAndOpacity(Color);
		return;
	}
	
	FLinearColor NewColor = Color;
	NewColor.A = OverlayOpacity;
	Overlay->SetColorAndOpacity(NewColor);

	UE_LOG(LogTemp, Warning, TEXT("Picked up Item") );
}
