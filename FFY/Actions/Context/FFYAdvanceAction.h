// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Actions/FFYAttackAction.h"
#include "FFYAdvanceAction.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API AFFYAdvanceAction : public AFFYAttackAction
{
	GENERATED_BODY()

	AFFYAdvanceAction();

public:
	virtual float GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner) override;
};
