#include "DummySurvivalCharacter.h"

#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h"

ADummySurvivalCharacter::ADummySurvivalCharacter()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetVisibleInSceneCaptureOnly(true);
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Capture Component"));
	CaptureComponent->SetupAttachment(SpringArm, SpringArm->SocketName);

	CaptureComponent->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	CaptureComponent->ShowOnlyComponent(Mesh);
}

void ADummySurvivalCharacter::RotateZ(float Amount)
{
	SpringArm->SetRelativeRotation(SpringArm->GetRelativeRotation() + FRotator{0,Amount,0});
}

