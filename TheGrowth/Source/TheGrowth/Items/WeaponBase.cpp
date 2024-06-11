#include "WeaponBase.h"

#include "TheGrowth/Components/EquipmentComponent.h"
#include "TheGrowth/Components/ItemComponent.h"
#include "ProjectileBase.h"
#include "TheGrowth/DataAssets/ItemData.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Attachments = CreateDefaultSubobject<UEquipmentComponent>(TEXT("Attachments"));
}

bool AWeaponBase::CanFitMagazine(UItemData* Magazine)
{
	bool CanFitMagazine{false};

	for(UItemData* ItemData : MagazineTypes)
	{
		if (ItemData == Magazine)
		{
			CanFitMagazine = true;
			break;
		}
	}

	return CanFitMagazine;
}

void AWeaponBase::Discharge()
{
	FString ChamberSlotID{"Chamber"};
	if (FItemStruct* ItemInChamber = Attachments->GetSlotByName(ChamberSlotID))
	{
		if (IsValid(ItemInChamber->ItemData) == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("Attempted to discharge invalid ammunition") );
			return;
		}
		
		AProjectileBase* SpawnedProjectile = GetWorld()->SpawnActor<AProjectileBase>(ItemInChamber->ItemData->AssociatedActor);
		SpawnedProjectile->SetActorTransform(MeshComponent->GetSocketTransform(FName("S_Muzzle")));
		SpawnedProjectile->SetDirectionOfMotion(GetActorForwardVector());
		Attachments->EmptyNamedSlot(ChamberSlotID);

		UE_LOG(LogTemp, Warning, TEXT("Fired Round") );
	}

	
	FString MagazineSlotID{"Magazine"};
	if (FItemStruct* CurrentMagazine = Attachments->GetSlotByName(MagazineSlotID))
	{
		if (IsValid(CurrentMagazine->ItemData) == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("Attempted to feed new round from invalid magazine") );
			return;
		}
	
		if (CurrentMagazine->Inventory.Num() > 0)
		{
			Attachments->PopulateNamedSlot(ChamberSlotID, CurrentMagazine->Inventory[0]);
			CurrentMagazine->Inventory.RemoveAt(0);
		}
	}
}

void AWeaponBase::Reload(FItemStruct& Magazine)
{
	if (IsValid(Magazine.ItemData) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to reload invalid magazine") );
		return;
	}
	
	FString MagazineSlotID{"Magazine"};
	Attachments->PopulateNamedSlot(MagazineSlotID, Magazine);

	if (Magazine.Inventory.Num() > 0)
	{
		FString ChamberSlotID{"Chamber"};
		Attachments->PopulateNamedSlot(ChamberSlotID, Magazine.Inventory[0]);
		Magazine.Inventory.RemoveAt(0);
	}
}
