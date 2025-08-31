// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/FFYAction.h"
#include "FFY/Actions/FFYAttackAction.h"
#include "FFYCounterAction.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API AFFYCounterAction : public AFFYAttackAction
{
	GENERATED_BODY()

public:
	AFFYCounterAction();

	virtual float GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner) override;
	virtual void ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets) override;
};
