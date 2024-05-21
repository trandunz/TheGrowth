// Copyright William Inman. All Rights Reserved.

#include "ItemBase.h"

#include "TheGrowth/Pawns/SurvivalCharacter.h"

AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh (Root)"));
	SetRootComponent(Mesh);

	Mesh->SetCollisionProfileName(FName("PhysicsActor"));
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	Mesh->SetSimulatePhysics(true);
}

void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBase::Interact(class ASurvivalCharacter* Character)
{
	if (IsValid(Character) == false)
		return;

	Character->PickupItem(this);
}

