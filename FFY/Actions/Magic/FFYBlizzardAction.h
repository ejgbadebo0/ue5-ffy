// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Actions/Magic/FFYSpellAction.h"
#include "FFYBlizzardAction.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API AFFYBlizzardAction : public AFFYSpellAction
{
	GENERATED_BODY()

	AFFYBlizzardAction();
	
public:
		
	virtual float GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner) override;
	virtual void Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex) override;
};
