#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "GenericDragDropOperation.generated.h"

UCLASS()
class THEGROWTH_API UGenericDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* WidgetReference{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D DragOffset{};
};
