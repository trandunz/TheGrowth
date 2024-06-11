#include "ProjectileBase.h"

#include "Components/ArrowComponent.h"
#include "Particles/ParticleSystemComponent.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Parent"));
	SetRootComponent(ArrowComponent);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachToComponent(ArrowComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	Mesh->SetRelativeRotation({0, -90.0f, 0.0f});
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	TracerParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Tracer Particle System"));
	TracerParticleSystem->AttachToComponent(ArrowComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	TracerParticleSystem->SetWorldRotation(FRotator{});

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
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_EngineTraceChannel2, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECR_Ignore);
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		if (Velocity.Length() > 10.0f)
		{
			TracerParticleSystem->Activate(true);
		
			FHitResult hitResult;
			FVector StartLineTrace = GetActorLocation();
			FVector EndLineTrace = (Velocity * DeltaTime) + StartLineTrace;
			FVector ForwardVector = EndLineTrace - StartLineTrace;	
			BulletAngle = FRotator(0.f, 0.0f, ForwardVector.Rotation().Pitch - GetActorRotation().Pitch);
			SetActorRotation(BulletAngle);
	
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(this);
 
			RotationPitch = GetActorRotation().Pitch * -1;
	
			if(EnableDrawDebugLine)
				DrawDebugLine(GetWorld(), StartLineTrace, EndLineTrace, FColor(0, 200.0f, 100.0f), true);
	
			if(GetWorld()->LineTraceSingleByChannel(hitResult, StartLineTrace, EndLineTrace, ECC_EngineTraceChannel2))
			{
				if (auto hitActor = hitResult.GetActor())
				{
					UE_LOG(LogTemp, Display, TEXT("Bullet Hit Something!"));
				}
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
			TracerParticleSystem->Deactivate();
			Mesh->SetCollisionProfileName("PhysicsActor");
			Mesh->SetSimulatePhysics(true);
		}
	}
}

void AProjectileBase::SetDirectionOfMotion(FVector NewDirection)
{
	Velocity = NewDirection.GetSafeNormal() * TravelSpeed;
}

