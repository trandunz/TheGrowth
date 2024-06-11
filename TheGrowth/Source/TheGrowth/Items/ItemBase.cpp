// Copyright William Inman. All Rights Reserved.

#include "ItemBase.h"

#include "TheGrowth/Components/ItemComponent.h"
#include "TheGrowth/Pawns/SurvivalCharacter.h"

AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh (Root)"));
	SetRootComponent(MeshComponent);
	MeshComponent->SetCollisionProfileName(FName("PhysicsActor"));

	ItemComponent = CreateDefaultSubobject<UItemComponent>(TEXT("Item Component"));
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	TogglePhysics(true);
}

void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBase::Interact(class ASurvivalCharacter* Character)
{
	if (IsValid(Character) == false)
		return;

	TogglePhysics(false);
	
	Character->PickupItem(this);
}

void AItemBase::TogglePhysics(bool Enabled)
{
	if (IsValid(MeshComponent) == false)
		return;
	
	if (Enabled)
	{
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MeshComponent->SetSimulatePhysics(true);
	}
	else
	{
		MeshComponent->SetSimulatePhysics(false);
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

