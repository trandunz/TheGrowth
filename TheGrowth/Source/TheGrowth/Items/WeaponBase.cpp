#include "WeaponBase.h"

#include "TheGrowth/Components/EquipmentComponent.h"
#include "TheGrowth/Components/ItemComponent.h"
#include "ProjectileBase.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "TheGrowth/DataAssets/FirearmData.h"
#include "TheGrowth/DataAssets/ItemData.h"
#include "TheGrowth/Pawns/SurvivalCharacter.h"


AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	bAllowTickBeforeBeginPlay = true;

	Attachments = CreateDefaultSubobject<UEquipmentComponent>(TEXT("Attachments"));
	Attachments->Slots.Add("Chamber", {});
	Attachments->Slots.Add("Magazine", {});
	
	MuzzleFlashVFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Muzzle Flash VFX"));
	MuzzleFlashVFX->SetupAttachment(MeshComponent, FName("S_Muzzle"));

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->SetupAttachment(MeshComponent);
	AudioComponent->bCanPlayMultipleInstances = true;
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	MuzzleFlashVFX->SetupAttachment(MeshComponent, FName("S_Muzzle"));
	MuzzleFlashVFX->DeactivateImmediate();
}

void AWeaponBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	DecrementFireRateTimer(DeltaSeconds);
	DecrementRecoilTimer(DeltaSeconds);
}

bool AWeaponBase::CanFitMagazine(UItemData* Magazine)
{
	if (IsValid(FirearmData) == false)
		return false;
	
	bool CanFitMagazine{false};

	for(UItemData* ItemData : FirearmData->CompatibleMagazines)
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
	if (FireRateTimer > 0)
		return;

	FItemStruct* ItemInChamber = Attachments->GetSlotByName(CHAMBER_SLOT_ID);
	if (ItemInChamber == nullptr)
		return;
	
	if (IsValid(ItemInChamber->ItemData) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to discharge empty chamber") );
		return;
	}
	if (IsValid(ItemInChamber->ItemData->AssociatedActor) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to discharge invalid ammunition") );
		return;
	}
		
	AProjectileBase* SpawnedProjectile = GetWorld()->SpawnActor<AProjectileBase>(ItemInChamber->ItemData->AssociatedActor);
	FTransform MuzzleTransform = MeshComponent->GetSocketTransform(FName("S_Muzzle"));
	SpawnedProjectile->SetActorTransform(MuzzleTransform);

	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FVector TraceStart = CameraManager->GetCameraLocation();
	FVector CameraForward = UKismetMathLibrary::GetForwardVector(CameraManager->GetCameraRotation());
	FVector TraceEnd = TraceStart + (CameraForward * 10000.0f);
	FCollisionQueryParams Params{};
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetAttachParentActor());
	FHitResult ForwardHitResult{};
	FVector DirectionOfMotion{TraceEnd - MuzzleTransform.GetLocation()};
	if(GetWorld()->LineTraceSingleByChannel(ForwardHitResult, TraceStart, TraceEnd , ECC_WorldDynamic, Params))
	{
		DirectionOfMotion = ForwardHitResult.Location - TraceStart;
	}
	
	SpawnedProjectile->SetDirectionOfMotion(DirectionOfMotion);
	SpawnedProjectile->ActorsToIngore.Add(this);
	SpawnedProjectile->ActorsToIngore.Add(GetAttachParentActor());
	Attachments->EmptyNamedSlot(CHAMBER_SLOT_ID);

	UE_LOG(LogTemp, Warning, TEXT("Fired Round") );

	MuzzleFlashVFX->Activate(true);
	PlayDischargeSound();
	FireRateTimer = 1.0f / (FirearmData->RPM / 60.0f);
	RecoilTimer = FirearmData->RecoilResetTime;
	CurrentRecoilIndex = (CurrentRecoilIndex + 1) % FirearmData->RecoilTable.Num();

	LoadChamberFromMagazine();
}

void AWeaponBase::Reload(FItemStruct& Magazine)
{
	if (IsValid(Magazine.ItemData) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to reload invalid magazine") );
		return;
	}

	LoadChamberFromMagazine(Magazine);
	Attachments->PopulateNamedSlot(MAGAZINE_SLOT_ID, Magazine);
	
	PlayReloadSound();
}

void AWeaponBase::LoadChamberFromMagazine()
{
	if (FItemStruct* CurrentMagazine = Attachments->GetSlotByName(MAGAZINE_SLOT_ID))
	{
		if (IsValid(CurrentMagazine->ItemData) == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("Attempted to feed new round from invalid magazine") );
			return;
		}
	
		if (CurrentMagazine->Inventory.Num() > 0)
		{
			Attachments->PopulateNamedSlot(CHAMBER_SLOT_ID, CurrentMagazine->Inventory[0]);
			CurrentMagazine->Inventory.RemoveAt(0);
			CurrentMagazine->Inventory.Shrink();
		}
	}
}

void AWeaponBase::LoadChamberFromMagazine(FItemStruct& Magazine)
{
	if (Magazine.Inventory.Num() > 0)
	{
		Attachments->PopulateNamedSlot(CHAMBER_SLOT_ID, Magazine.Inventory[0]);
		Magazine.Inventory.RemoveAt(0);
		Magazine.Inventory.Shrink();
	}
}

void AWeaponBase::DecrementFireRateTimer(float DeltaSeconds)
{
	if (FireRateTimer > 0)
	{
		FireRateTimer -= DeltaSeconds;
		
		if (FireRateTimer < 0)
			FireRateTimer = 0.0f;
	}
}

void AWeaponBase::DecrementRecoilTimer(float DeltaSeconds)
{
	if (RecoilTimer > 0)
	{
		RecoilTimer -= DeltaSeconds;
		
		if (RecoilTimer <= 0)
		{
			RecoilTimer = 0.0f;
			CurrentRecoilIndex = 0;
		}
	}
}

void AWeaponBase::PlayDischargeSound()
{
	if (IsValid(FirearmData) == false)
		return;
	
	if (IsValid(FirearmData->DischargeCue) == false)
		return;

	if (IsValid(FirearmData->DischargeAttenuation) == false)
		return;

	AudioComponent->SetSound(FirearmData->DischargeCue);
	AudioComponent->AttenuationSettings = FirearmData->DischargeAttenuation;
	AudioComponent->Play();
}

void AWeaponBase::PlayReloadSound()
{
	if (IsValid(FirearmData) == false)
		return;
	
	if (IsValid(FirearmData->ReloadCue) == false)
		return;

	if (IsValid(FirearmData->ReloadAttenuation) == false)
		return;
	
	AudioComponent->SetSound(FirearmData->ReloadCue);
	AudioComponent->AttenuationSettings = FirearmData->ReloadAttenuation;
	AudioComponent->Play();
}
