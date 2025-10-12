// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Actions/FFYAttackAction.h"
#include "FFYTailSwipeAction.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API AFFYTailSwipeAction : public AFFYAttackAction
{
	GENERATED_BODY()

	AFFYTailSwipeAction();

public:
	virtual float GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner) override;
	virtual void Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex) override;
};
