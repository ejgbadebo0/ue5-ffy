// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Actions/FFYAttackAction.h"
#include "FFYFinishAction.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API AFFYFinishAction : public AFFYAttackAction
{
	GENERATED_BODY()

	AFFYFinishAction();

public:
	virtual float GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner) override;
	virtual void Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex) override;
};
