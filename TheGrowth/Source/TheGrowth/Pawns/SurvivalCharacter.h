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
	void StartFreeLook();
	void EndFreeLook();
	
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

	UFUNCTION()
	void UpdateZoomTimeline(float Delta);
	UFUNCTION()
	void UpdateCameraResetTimeline(float Delta);
	UFUNCTION()
	void OnCameraResetTimelineFinish();
	
protected: // Camera Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= CameraSettings, meta = (AllowPrivateAccess = "true"))
	float MaxBoomLength{400.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= CameraSettings, meta = (AllowPrivateAccess = "true"))
	float MinBoomLength{200.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= CameraSettings, meta = (AllowPrivateAccess = "true"))
	float ScrollZoomStrength{50.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= CameraSettings, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* ZoomCurve{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= CameraSettings, meta = (AllowPrivateAccess = "true"))
	float ZoomFOV{70.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= CameraSettings, meta = (AllowPrivateAccess = "true"))
	float DefaultFOV{90.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= CameraSettings, meta = (AllowPrivateAccess = "true"))
	float ZoomTime{0.2f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= CameraSettings, meta = (AllowPrivateAccess = "true"))
	bool bFirstPerson{false};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= CameraSettings, meta = (AllowPrivateAccess = "true"))
	bool bFreeLooking{false};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= CameraSettings, meta = (AllowPrivateAccess = "true"))
	float FreeLookStartYaw{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= CameraSettings, meta = (AllowPrivateAccess = "true"))
	float FreeLookEndYaw{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= CameraSettings, meta = (AllowPrivateAccess = "true"))
	float FreeLookYawExtent{120.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= CameraSettings, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* CameraResetCurve{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= CameraSettings, meta = (AllowPrivateAccess = "true"))
	float CameraResetTime{0.2f};
	
	
protected: // References
	UPROPERTY(BlueprintReadOnly, Category= References)
	class APlayerController* PlayerController{nullptr};
	UPROPERTY(BlueprintReadOnly, Category= References)
	class ASurvivalPlayerState* PlayerStateRef{nullptr};
	UPROPERTY(BlueprintReadOnly, Category= References)
	class USurvivalMovementComponent* MovementComponent{nullptr};
	
protected: // Components //
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Camera, meta = (AllowPrivateAccess = "true"))
	class UTimelineComponent* ZoomTimeline{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Camera, meta = (AllowPrivateAccess = "true"))
	class UTimelineComponent* CameraResetTimeline{nullptr};

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
	class UInputAction* SprintAction{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AimAction{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ZoomAction{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FreeLookAction{nullptr};
};

