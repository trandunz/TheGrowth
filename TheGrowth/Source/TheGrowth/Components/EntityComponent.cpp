// Copyright William Inman. All Rights Reserved.

#include "EntityComponent.h"

#include "TheGrowth/Pawns/CharacterBase.h"

UEntityComponent::UEntityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEntityComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = GetOwner<ACharacterBase>();

	for(int32 I = 0; I < CurrentBodyPartHealth.Num(); I++)
	{
		CurrentBodyPartHealth[(EBodyPart)I] = MaxBodyPartHealth[(EBodyPart)I];
	}
}

void UEntityComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RegenerateStamina(DeltaTime);
	BleedOut(DeltaTime);
}

EBodyPart UEntityComponent::ConvertBoneToBodyPart(FString Bone)
{
	if (Bone == "head")
	{
		return HEAD;
	}
	else if (Bone == "neck_01")
	{
		return HEAD;
	}
	else if (Bone == "spine_03")
	{
		return CHEST;
	}
	else if (Bone == "spine_01")
	{
		return STOMACH;
	}
	else if (Bone == "upperarm_r")
	{
		return ARM_R;
	}
	else if (Bone == "upperarm_l")
	{
		return ARM_L;
	}
	else if (Bone == "hand_r")
	{
		return ARM_R;
	}
	else if (Bone == "hand_l")
	{
		return ARM_L;
	}
	else if (Bone == "thigh_r")
	{
		return LEG_R;
	}
	else if (Bone == "thigh_l")
	{
		return LEG_L;
	}
	else if (Bone == "foot_r")
	{
		return LEG_R;
	}
	else if (Bone == "foot_l")
	{
		return LEG_L;
	}
	else
	{
		return HEAD;
	}
}

void UEntityComponent::OffsetHealth(EBodyPart BodyPart, float Amount, bool ApplyBleeding)
{
	CurrentBodyPartHealth[BodyPart].Health = FMath::Clamp(CurrentBodyPartHealth[BodyPart].Health + Amount, 0.0f, MaxBodyPartHealth[BodyPart].Health);

	if(ApplyBleeding)
		CurrentBodyPartHealth[BodyPart].bBleeding = true;
	
	if (CurrentBodyPartHealth[BodyPart].Health <= 0)
		BloodVolume = FMath::Clamp(BloodVolume - (ArteryPunctureVolumeRate), 0.0f, BloodVolume);
	
	UE_LOG(LogTemp, Warning, TEXT("%s Took %s Damage. New HP = %s"), *GetOwner()->GetName(), *FString::SanitizeFloat(Amount, 0), *FString::SanitizeFloat(CurrentBodyPartHealth[BodyPart].Health, 0));
}

void UEntityComponent::BleedOut(float DeltaTime)
{
	// Prevent Bleeding If Already Dead
	if (IsDead())
		return;

	// Apply Bleeding
	for(int32 I = 0; I < CurrentBodyPartHealth.Num(); I++)
	{
		if (CurrentBodyPartHealth[(EBodyPart)I].Health <= 0.0f)
		{
			BloodVolume = FMath::Clamp(BloodVolume - (DeltaTime * ArteryPunctureVolumeRate), 0.0f, BloodVolume);
		}
		else if (CurrentBodyPartHealth[(EBodyPart)I].bBleeding)
		{
			BloodVolume = FMath::Clamp(BloodVolume - (DeltaTime * FleshWoundDamageRate), 0.0f, BloodVolume);
			CurrentBodyPartHealth[(EBodyPart)I].Health = FMath::Clamp(CurrentBodyPartHealth[(EBodyPart)I].Health - (DeltaTime * FleshWoundDamageRate), 0.0f, CurrentBodyPartHealth[(EBodyPart)I].Health);
		}
	}

	// Check if bleeding killed the entity
	if (IsValid(OwningCharacter) && IsDead())
	{
		OwningCharacter->SetRagdoll(true);
	}
}

void UEntityComponent::OffsetStamina(float Amount)
{
	bShouldRegenStamina = false;
	CurrentStamina = FMath::Clamp(CurrentStamina + Amount, 0, MaxStamina);

	if (auto World = GetWorld())
	{
		World->GetTimerManager().SetTimer(StaminaRegenTimerHandle, this, &UEntityComponent::BeginStaminaReset, StaminaRegenerationDelay);
	}
	
}

bool UEntityComponent::IsDead() const
{
	return GetTotalCurrentHealth() <= 0 ||
		CurrentBodyPartHealth[HEAD].Health <= 0 ||
			CurrentBodyPartHealth[CHEST].Health <= 0 ||
				BloodVolume <= MinimumBloodVolume;
}

void UEntityComponent::BeginStaminaReset()
{
	bShouldRegenStamina = true;
}

void UEntityComponent::RegenerateStamina(float DeltaSeconds)
{
	if (bShouldRegenStamina == false)
		return;
	
	CurrentStamina = FMath::Clamp(CurrentStamina + (DeltaSeconds * StaminaRegenerationSpeed), 0, MaxStamina);
}

float UEntityComponent::GetTotalCurrentHealth() const
{
	float TotalCurrentHealth{};
	
	for(auto BodyPart : CurrentBodyPartHealth)
		TotalCurrentHealth += BodyPart.Value.Health;
	
	return TotalCurrentHealth;
}

float UEntityComponent::GetTotalMaxHealth() const
{
	float TotalCurrentHealth{};
	
	for(auto BodyPart : MaxBodyPartHealth)
		TotalCurrentHealth += BodyPart.Value.Health;
	
	return TotalCurrentHealth;
}