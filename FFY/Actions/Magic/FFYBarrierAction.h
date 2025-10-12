// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Actions/Magic/FFYSpellAction.h"
#include "FFYBarrierAction.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API AFFYBarrierAction : public AFFYSpellAction
{
	GENERATED_BODY()

	AFFYBarrierAction();

public:

	virtual void Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex) override;
};

