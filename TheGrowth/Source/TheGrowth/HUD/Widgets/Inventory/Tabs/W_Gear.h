#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Templates/Tuple.h"
#include "TheGrowth/DataAssets/ItemData.h"
#include "W_Gear.generated.h"

UCLASS()
class THEGROWTH_API UW_Gear : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeOnInitialized() override;
	
public:
	UFUNCTION()
	bool CanPickupItem(class AItemBase* Item);
	
	TTuple<class UW_InventoryContainer*, int, FVector2D> PickupItem(class AItemBase* Item);
	void RemoveItem(FItemStruct& Item);

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class UW_InventoryContainer*> Containers{};
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UW_InventoryContainer* Earpiece{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UW_InventoryContainer* Head{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UW_InventoryContainer* Face{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UW_InventoryContainer* Arm{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UW_InventoryContainer* Body{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UW_InventoryContainer* EyeWear{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UW_InventoryContainer* Holster{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UW_InventoryContainer* Melee{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UW_InventoryContainer* Primary{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UW_InventoryContainer* Secondary{nullptr};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UW_InventoryContainer* Rig{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UW_InventoryContainer* Pockets{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UW_InventoryContainer* Specials{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UW_InventoryContainer* Backpack{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UW_InventoryContainer* Pouch{nullptr};
};
