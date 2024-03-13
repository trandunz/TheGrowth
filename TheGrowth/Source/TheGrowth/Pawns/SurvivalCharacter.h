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
	
protected: // Input // 
	void Move(const struct FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Scroll(const FInputActionValue& Value);
	void StartAim();
	void EndAim();
	void StartZoom();
	void EndZoom();
	
public:
	UFUNCTION()
	void OffsetHealth(float Amount);

protected:
	UFUNCTION()
	void UpdateBoomLength(float Increment);
	UFUNCTION()
	void SetPerspective(bool FirstPerson);

	UFUNCTION()
	void SetRagdoll(bool Ragdoll);
	
protected: // Camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Camera, meta = (AllowPrivateAccess = "true"))
	float MaxBoomLength{400.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Camera, meta = (AllowPrivateAccess = "true"))
	float MinBoomLength{200.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Camera, meta = (AllowPrivateAccess = "true"))
	float ScrollZoomStrength{50.0f};
	
protected: // References
	UPROPERTY(BlueprintReadOnly, Category= References)
	class APlayerController* PlayerController{nullptr};
	UPROPERTY(BlueprintReadOnly, Category= References)
	class ASurvivalPlayerState* PlayerStateRef{nullptr};
	
protected: // Components //
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera{nullptr};


protected: // Input //
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ScrollAction{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AimAction{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ZoomAction{nullptr};
};

