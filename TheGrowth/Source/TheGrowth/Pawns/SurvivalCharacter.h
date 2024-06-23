// Copyright William Inman. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "TheGrowth/DataAssets/ItemData.h"
#include "SurvivalCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ASurvivalCharacter : public ACharacter
{
	GENERATED_BODY()
	friend class AItemBase;
	friend class UW_SurvivalHUD;
	friend class ASurvivalHUD;
	friend class UW_InventoryItem;

protected:
	ASurvivalCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
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
	void ToggleCrouch();
	
	void StartLean(bool Left);
	void EndLean(bool Left);

	void Jump() override;

	void Prone();

	void StartSprint();
	void EndSprint();
	void UpdateStaminaFromSprint(float DeltaSeconds);

public:
	UFUNCTION()
	void OffsetHealth(float Amount);

	UFUNCTION(BlueprintCallable)
	FTransform GetLeftHandSocketTransform();

	UFUNCTION()
	void OnWindowFocusChanged(bool bIsFocused);

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
	UFUNCTION()
	void UpdateLeanAngle(float DeltaSeconds);

	UFUNCTION()
	void ToggleInventoryWidget();
	UFUNCTION()
	void SetGearWidgetRef(class UW_Gear* GearWidget);

	UFUNCTION()
	void CheckForInteractables();
	UFUNCTION()
	void TryInteract();

	UFUNCTION()
	bool PickupItem(class AItemBase* Item);

	UFUNCTION()
	void RemoveItem(FItemStruct& Item);

	UFUNCTION()
	void UpdateAimTimeline(float Delta);

	UFUNCTION()
	void OnAimTimelineFinished();

	UFUNCTION()
	void Attack();

	UFUNCTION()
	void Reload();
	

protected: // Interaction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Interaction, meta = (AllowPrivateAccess = "true"))
	float InteractRange{200.0f};
	class IInteractInterface* LastInteractable{};

protected: // UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= UI, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ADummySurvivalCharacter> DummyCharacterPrefab{nullptr};
protected:
	UPROPERTY(BlueprintReadOnly, Category= Firearms)
	float AimDelta{};
	UPROPERTY(BlueprintReadOnly, Category= Firearms)
	float AimTime{0.2f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Firearms, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* AimCurve{nullptr};
	
	//TEMP
	UPROPERTY(EditDefaultsOnly, Category= Firearms)
	TSubclassOf<class AWeaponBase> TestWeaponPrefab{};
	
protected: // Movement Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Movement, meta = (AllowPrivateAccess = "true"))
	float MaximumLeanAngle{25.0f};
	UPROPERTY(BlueprintReadOnly, Category= Movement)
	float LeanAngle{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Movement, meta = (AllowPrivateAccess = "true"))
	float LeanSpeed{200.0f};
	UPROPERTY(BlueprintReadOnly, Category= Movement)
	float LeanInput{};
	
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
	UPROPERTY(BlueprintReadOnly, Category= References)
	class ASurvivalHUD* HUDRef{nullptr};
	UPROPERTY(BlueprintReadOnly, Category= References)
	class UW_Gear* GearRef{nullptr};
	UPROPERTY(BlueprintReadOnly, Category= References)
	class AWeaponBase* ActiveWeaponRef{nullptr};
	
protected: // Components //
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Components, meta = (AllowPrivateAccess = "true"))
	class UTimelineComponent* ZoomTimeline{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Components, meta = (AllowPrivateAccess = "true"))
	class UTimelineComponent* CameraResetTimeline{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Components, meta = (AllowPrivateAccess = "true"))
	class UTimelineComponent* AimTimeline{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Components, meta = (AllowPrivateAccess = "true"))
    class ADummySurvivalCharacter* DummyCharacter{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Components, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComponent{nullptr};

protected: // Input //
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* OOCMappingContext{nullptr};

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CrouchAction{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ProneAction{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReloadAction{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LeanActionLeft{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LeanActionRight{nullptr};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction{nullptr};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ToggleInventoryAction{nullptr};
};

