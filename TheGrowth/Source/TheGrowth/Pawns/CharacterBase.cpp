#include "CharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "TheGrowth/Components/EntityComponent.h"
#include "TheGrowth/Items/ProjectileBase.h"

ACharacterBase::ACharacterBase()
{
	EntityComponent = CreateDefaultSubobject<UEntityComponent>(TEXT("Entity Component"));
	
	if (IsValid(GetCapsuleComponent()) == false)
		return;
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
}

void ACharacterBase::SetRagdoll(bool Ragdoll) const
{
	if (IsValid(Controller) == false)
		return;

	if (IsValid(GetMesh()) == false)
		return;
	
	if (Ragdoll)
	{
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->WakeAllRigidBodies();
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		
		if (IsValid(Controller))
		{
			Controller->SetIgnoreMoveInput(true);
			Controller->UnPossess();
		}
	}
	else
	{
		GetMesh()->SetAllBodiesSimulatePhysics(false);
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->WakeAllRigidBodies();
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		
		if (IsValid(Controller))
		{
			Controller->SetIgnoreMoveInput(false);
		}
	}
}

void ACharacterBase::TakeDamage(FString Bone, AProjectileBase* Projectile)
{
	const EBodyPart HitBodyPart = UEntityComponent::ConvertBoneToBodyPart(Bone);
	if (IsDead() == false)
	{
		EntityComponent->OffsetHealth(HitBodyPart, -Projectile->GetDamage());

		// Check If It Killed Player //
		if (IsDead())
		{
			SetRagdoll(true);
		}
	}
}

void ACharacterBase::TakeDamage(FString Bone, float Amount)
{
	const EBodyPart HitBodyPart = UEntityComponent::ConvertBoneToBodyPart(Bone);
	if (IsDead() == false)
	{
		EntityComponent->OffsetHealth(HitBodyPart, -Amount);

		// Check If It Killed Player //
		if (IsDead())
		{
			SetRagdoll(true);
		}
	}
}

void ACharacterBase::TakeDamage(float Amount)
{
	if (IsDead() == false)
	{
		EntityComponent->OffsetHealth(HEAD, -Amount);

		// Check If It Killed Player //
		if (IsDead())
		{
			SetRagdoll(true);
		}
	}
}

void ACharacterBase::OffsetStamina(float Amount)
{
	EntityComponent->OffsetStamina(Amount);
}

bool ACharacterBase::IsDead() const
{
	return EntityComponent->IsDead();
}
