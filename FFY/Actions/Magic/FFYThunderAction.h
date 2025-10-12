// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Actions/Magic/FFYSpellAction.h"
#include "FFYThunderAction.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API AFFYThunderAction : public AFFYSpellAction
{
	GENERATED_BODY()

	AFFYThunderAction();
	
public:
		
	virtual float GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner) override;
	virtual void Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex) override;
};
