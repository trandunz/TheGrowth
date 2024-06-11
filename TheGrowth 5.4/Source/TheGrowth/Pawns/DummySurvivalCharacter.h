#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DummySurvivalCharacter.generated.h"

UCLASS()
class THEGROWTH_API ADummySurvivalCharacter : public AActor
{
	GENERATED_BODY()
	
	ADummySurvivalCharacter();

public:
	UFUNCTION()
	void RotateZ(float Amount);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float RotationStartZ{};
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* Root{nullptr};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* Mesh{nullptr};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneCaptureComponent2D* CaptureComponent{nullptr};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USpringArmComponent* SpringArm{nullptr};

};
