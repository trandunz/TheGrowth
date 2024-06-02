// Copyright William Inman. All Rights Reserved.

#include "W_InventorySlot.h"
#include "Components/Image.h"

int32 UW_InventorySlot::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                    const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
                                    const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	DrawBorderOutline(AllottedGeometry, OutDrawElements, LayerId);
	
	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                          bParentEnabled);
}

void UW_InventorySlot::DrawBorderOutline(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements,
	int32 LayerId) const
{
	FVector2D TopLeft;
	TopLeft.X = 0;
	TopLeft.Y = 0;

	FVector2D BottomRight;
	BottomRight.X = GetDesiredSize().X;
	BottomRight.Y = GetDesiredSize().Y;

	TArray< FVector2D > Vertices;
	Vertices.AddUninitialized(5);
	Vertices[0] = FVector2D(TopLeft.X, TopLeft.Y);
	Vertices[1] = FVector2D(TopLeft.X, BottomRight.Y);
	Vertices[2] = FVector2D(BottomRight.X, BottomRight.Y);
	Vertices[3] = FVector2D(BottomRight.X, TopLeft.Y);
	Vertices[4] = FVector2D(TopLeft.X, TopLeft.Y);

	FSlateDrawElement::MakeLines(
	OutDrawElements,
	LayerId,
	AllottedGeometry.ToPaintGeometry(),
	Vertices,
	ESlateDrawEffect::None,
	FLinearColor(0.85f, 0.85f, 0.85f, 1.0f),
	true,
	0.5f
	);
}

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
