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
	
	void Crouch(bool bClientSimulation) override;
	void UnCrouch(bool bClientSimulation) override;

	void Prone();
	void UnProne();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Movement, meta = (AllowPrivateAccess = "true"))
	float MaxJogSpeed{500.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Movement, meta = (AllowPrivateAccess = "true"))
	float MaxSprintSpeed{900.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Movement, meta = (AllowPrivateAccess = "true"))
	float MaxCrouchSpeed{300.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Movement, meta = (AllowPrivateAccess = "true"))
	float MaxProneSpeed{100.0f};
};
