#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SurvivalMovementComponent.generated.h"

UCLASS()
class THEGROWTH_API USurvivalMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	friend class ASurvivalCharacter;

	virtual void BeginPlay() override;
	
protected:
	void StartSprint();
	void EndSprint();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Movement, meta = (AllowPrivateAccess = "true"))
	float JogSpeed{500.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Movement, meta = (AllowPrivateAccess = "true"))
	float SprintSpeed{900.0f};
};
