// Copyright William Inman. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "SurvivalCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ASurvivalCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	ASurvivalCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	void Move(const struct FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Scroll(const FInputActionValue& Value);

protected:
	UFUNCTION()
	void UpdateBoomLength(float Increment);

	UFUNCTION()
	void SetPerspective(bool FirstPerson);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Camera, meta = (AllowPrivateAccess = "true"))
	float MaxBoomLength{400.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Camera, meta = (AllowPrivateAccess = "true"))
	float MinBoomLength{200.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Camera, meta = (AllowPrivateAccess = "true"))
	float ScrollZoomStrength{50.0f};
	
protected: // Components //
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

protected: // Input //
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ScrollAction;
};

