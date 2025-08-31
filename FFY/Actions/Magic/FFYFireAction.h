// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFYSpellAction.h"
#include "FFY/FFYAction.h"
#include "FFYFireAction.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API AFFYFireAction : public AFFYSpellAction
{
	GENERATED_BODY()

public:
	AFFYFireAction();

	/*
	virtual FName GetLabel() const override
	{
		return "Fire";
	}

	virtual FText GetDescription() const override
	{
		return FText::FromString("Deal a small amount of Fire damage to a target.");
	}

	virtual uint8 GetType() const override
	{
		return 0;
	}

	virtual ETargetType GetTargetType() const override
	{
		return ETargetType::BOTH;
	}

	virtual EActionType GetActionType() const override
	{
		return EActionType::MAGIC;
	}

	virtual bool GetIsQueuedAction() const override
	{
		return true;
	}

	virtual float GetATBCost() const override
	{
		return 50;
	}

	virtual float GetHPCost() const override
	{
		return 0;
	}

	virtual float GetMPCost() const override
	{
		return 5;
	}

	virtual float GetLPCost() const override
	{
		return 0;
	}

	virtual float GetRank() const override
	{
		return 1;
	}
	*/

	virtual float GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner) override;
	virtual void ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets) override;
	virtual void ExecuteAction(FBattleCharacterData& ActionOwner, TArray<FBattleCharacterData>& Targets) override;
	virtual void Redirect(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets) override;
	virtual void Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex) override;
};
