// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Actions/FFYAttackAction.h"
#include "FFYBlitzAction.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API AFFYBlitzAction : public AFFYAttackAction
{
	GENERATED_BODY()

public:
	AFFYBlitzAction();

	virtual float GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner) override;
	virtual void Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex) override;
	
};
