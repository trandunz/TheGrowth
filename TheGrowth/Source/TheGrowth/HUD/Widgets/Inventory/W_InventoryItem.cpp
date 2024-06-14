#include "W_InventoryItem.h"

#include "W_InventoryContainer.h"
#include "Components/Border.h"
#include "TheGrowth/Pawns/SurvivalCharacter.h"

void UW_InventoryItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	CharacterReference = GetOwningPlayerPawn<ASurvivalCharacter>();

	if (IsValid(CharacterReference))
		UE_LOG(LogTemp, Warning, TEXT("Character Reference Valid (UW_InventoryItem)") );
}

int32 UW_InventoryItem::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                    const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
                                    const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	DrawBorderOutline(AllottedGeometry, OutDrawElements, LayerId);
	
	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                          bParentEnabled);
}

void UW_InventoryItem::DrawBorderOutline(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements,
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

void UW_InventoryItem::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	BG->SetBrushColor({0.2,0.2,0.2,0.5});
}

void UW_InventoryItem::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	BG->SetBrushColor({0.0,0.0,0.0,0.5});
}

void UW_InventoryItem::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	UGenericDragDropOperation* DragWidget = Cast<UGenericDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UGenericDragDropOperation::StaticClass()));
	SetVisibility(ESlateVisibility::HitTestInvisible);
	DragWidget->Payload = this;
	DragWidget->DragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	DragWidget->DefaultDragVisual = this;

	OutOperation = DragWidget;

	UE_LOG(LogTemp, Warning, TEXT("Drag Item Attempt") );

	if (IsValid(CharacterReference))
	{
		// CAUSING CRASH
		CharacterReference->RemoveItem(ItemStruct);
	}
	
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

FReply UW_InventoryItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply ReplyResult = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	return ReplyResult.NativeReply;
}
