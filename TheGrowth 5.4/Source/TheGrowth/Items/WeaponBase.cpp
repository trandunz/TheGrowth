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

void AWeaponBase::Discharge()
{
	FString ChamberSlotID{"Chamber"};
	if (UItemComponent* ItemInChamber = Attachments->GetSlotByName(ChamberSlotID))
	{
		AProjectileBase* SpawnedProjectile = GetWorld()->SpawnActor<AProjectileBase>(ItemInChamber->ItemData->AssociatedActor);
		SpawnedProjectile->SetActorTransform(MeshComponent->GetSocketTransform(FName("S_Muzzle")));
		SpawnedProjectile->SetDirectionOfMotion(GetActorForwardVector());
		Attachments->EmptyNamedSlot(ChamberSlotID);
	}
}

void AWeaponBase::Reload()
{
}
