#include "ProjectileBase.h"

#include "NiagaraComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Parent"));
	SetRootComponent(ArrowComponent);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(ArrowComponent);
	Mesh->SetRelativeRotation({0, -90.0f, 0.0f});
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bReplicates = true;
	SetReplicatingMovement(true);
	bAlwaysRelevant = true;
	ArrowComponent->SetIsReplicated(true);
}

void AProjectileBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	SetReplicateMovement(true);
	ForceNetRelevant();
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		if (Velocity.Length() > 10.0f)
		{
			FHitResult hitResult;
			FVector StartLineTrace = GetActorLocation();
			FVector EndLineTrace = (Velocity * DeltaTime) + StartLineTrace;
			FVector ForwardVector = EndLineTrace - StartLineTrace;	
			SetActorRotation(ForwardVector.Rotation());

			if (bDrawDebugTracer)
				DrawDebugLine(GetWorld(), StartLineTrace, EndLineTrace, FColor::Purple, false, DebugTracerLifetime, 0, DebugTracerWidth);
	
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(this);
 
			RotationPitch = GetActorRotation().Pitch * -1;

			FCollisionQueryParams Params{};
			Params.AddIgnoredActors(ActorsToIngore);
			if(GetWorld()->LineTraceSingleByChannel(hitResult, StartLineTrace, EndLineTrace, ECC_WorldDynamic, Params))
			{
				if (auto hitActor = hitResult.GetActor())
				{
					UE_LOG(LogTemp, Display, TEXT("Bullet Hit %s!"), *hitActor->GetName());
					
				}

				if (IsValid(BulletImpactVFX))
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletImpactVFX, hitResult.Location, hitResult.Normal.Rotation());

				Destroy();
			}
	
			BulletTime += DeltaTime;
			SetActorLocation(EndLineTrace);
			if(!(120.0f >= RotationPitch && RotationPitch <= 70.0f))
			{
				RotationPitch = {};
			}
			Velocity.Z += Gravity;

			if(BulletTime > BulletLifetime)
			{
				Destroy();
				
			}
		}
		else
		{
			Mesh->SetCollisionProfileName("PhysicsActor");
			Mesh->SetSimulatePhysics(true);
		}
	}
}

void AProjectileBase::SetDirectionOfMotion(FVector NewDirection)
{
	Velocity = NewDirection.GetSafeNormal() * TravelSpeed;
}

