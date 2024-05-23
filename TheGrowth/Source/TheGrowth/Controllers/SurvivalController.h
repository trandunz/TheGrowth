#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SurvivalController.generated.h"

UCLASS()
class THEGROWTH_API ASurvivalController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
};
