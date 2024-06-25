#include "AICharacter.h"

#include "Runtime/AIModule/Classes/AIController.h"

AAICharacter::AAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AIController = Cast<AAIController>(Controller);
}

