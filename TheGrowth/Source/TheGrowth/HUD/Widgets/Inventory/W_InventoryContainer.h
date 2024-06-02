#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_InventoryContainer.generated.h"

UCLASS()
class THEGROWTH_API UW_InventoryContainer : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativePreConstruct() override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Inventory")
	FString ContainerTitle{"CONTAINER"};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Inventory")
	class UInventoryContainerData* ContainerData{};
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Inventory")
	TArray<class UW_InventorySlotCollection*> SlotCollections{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Inventory")
	TArray<class UW_InventorySlot*> Slots{};

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (BindWidget))
	class UNamedSlot* ContainerWidget{};

};
