// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Actions/Magic/FFYSpellAction.h"
#include "FFYEsunaAction.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API AFFYEsunaAction : public AFFYSpellAction
{
	GENERATED_BODY()

	AFFYEsunaAction();

public:
	
	virtual void Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex) override;
	virtual void ExecuteAction(FBattleCharacterData& ActionOwner, FBattleCharacterData& Target) override;
};
