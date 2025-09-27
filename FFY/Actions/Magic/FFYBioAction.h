// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Actions/Magic/FFYSpellAction.h"
#include "FFYBioAction.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API AFFYBioAction : public AFFYSpellAction
{
	GENERATED_BODY()
	
	AFFYBioAction();

public:
	virtual float GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner) override;
	virtual void Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex) override;
};
