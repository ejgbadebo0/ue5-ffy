// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Actions/FFYAttackAction.h"
#include "FFYPoisonNeedleAction.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API AFFYPoisonNeedleAction : public AFFYAttackAction
{
	GENERATED_BODY()
	
	

public:
	AFFYPoisonNeedleAction();
	
	virtual void Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex) override;
};
