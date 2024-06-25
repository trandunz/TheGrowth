#pragma once

#include "CoreMinimal.h"
#include "TheGrowth/Pawns/CharacterBase.h"
#include "AICharacter.generated.h"

UCLASS()
class THEGROWTH_API AAICharacter : public ACharacterBase
{
	GENERATED_BODY()
	
protected:
	AAICharacter();
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly, Category= References)
	class AAIController* AIController{nullptr};
};
