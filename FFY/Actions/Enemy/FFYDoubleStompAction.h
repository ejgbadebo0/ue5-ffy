// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/FFYAction.h"
#include "FFY/Actions/FFYAttackAction.h"
#include "FFYDoubleStompAction.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API AFFYDoubleStompAction : public AFFYAttackAction
{
	GENERATED_BODY()

	AFFYDoubleStompAction();

public:
	virtual void ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets) override;
	virtual void Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex) override;
};
