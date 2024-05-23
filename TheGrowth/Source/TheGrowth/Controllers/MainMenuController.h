#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuController.generated.h"

UCLASS()
class THEGROWTH_API AMainMenuController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
};
