#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SurvivalMovementComponent.generated.h"

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None	UMETA(Hidden),
	CMOVE_Prone	UMETA(DisplayName = "Prone"),
	CMOVE_MAX	UMETA(Hidden)
};

UCLASS()
class THEGROWTH_API USurvivalMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	friend class ASurvivalCharacter;

	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
	bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;
	bool IsMovementMode(EMovementMode InMovementMode) const;

	virtual bool CanCrouchInCurrentState() const override;
	virtual float GetMaxSpeed() const override;
	
protected:
	void StartSprint();
	void EndSprint();
	
	void Crouch(bool bClientSimulation) override;
	void UnCrouch(bool bClientSimulation) override;
	
	UFUNCTION(Server, Reliable)
	void Server_EnterProne();
	void Prone();
	void EnterProne();
	void ExitProne();
	bool CanProne() const;
	void PhysProne(float DeltaSeconds, int32 Iterations);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Movement, meta = (AllowPrivateAccess = "true"))
	float MaxJogSpeed{500.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Movement, meta = (AllowPrivateAccess = "true"))
	float MaxSprintSpeed{900.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Movement, meta = (AllowPrivateAccess = "true"))
	float MaxCrouchSpeed{300.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Movement, meta = (AllowPrivateAccess = "true"))
	float MaxProneSpeed{100.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Movement, meta = (AllowPrivateAccess = "true"))
	float ProneEnterImpulse{300.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Movement, meta = (AllowPrivateAccess = "true"))
	float ProneBrakingDeceleration{2500.0f};
	
protected:
	class FSavedMove_Survival : public FSavedMove_Character
	{
		typedef FSavedMove_Character Super;

		// Uint8 with length of 1 bit
		uint8 Saved_bWantsToSprint:1 {};
		uint8 Saved_bWantsToProne:1 {};

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	class FNetworkPredictionData_Client_Survival : public FNetworkPredictionData_Client_Character
	{
		typedef FNetworkPredictionData_Client_Character Super;
		
	public:
		FNetworkPredictionData_Client_Survival(const UCharacterMovementComponent& ClientMovement);

		virtual FSavedMovePtr AllocateNewMove() override;
	};

public:
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual bool IsMovingOnGround() const override;

	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Movement, meta = (AllowPrivateAccess = "true"))
	bool Safe_bWantsToSprint{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Movement, meta = (AllowPrivateAccess = "true"))
	bool Safe_bWantsToProne{};
};
