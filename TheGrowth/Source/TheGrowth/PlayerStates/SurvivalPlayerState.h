// Copyright William Inman. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SurvivalPlayerState.generated.h"

UCLASS()
class THEGROWTH_API ASurvivalPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	ASurvivalPlayerState();

public:
	UFUNCTION()
	void OffsetHealth(float Amount);
	UFUNCTION()
	void OffsetStamina(float Amount);
	UFUNCTION()
	bool IsDead() const;

public: // Setters & Getters //
	UFUNCTION()
	FORCEINLINE class UEntityComponent* GetEntityComponent() const {return EntityComponent;} 
	
protected: // Components //
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Components)
	class UEntityComponent* EntityComponent{nullptr};
};
