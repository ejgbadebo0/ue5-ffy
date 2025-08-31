// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFYSpellAction.h"
#include "FFYRaiseAction.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API AFFYRaiseAction : public AFFYSpellAction
{
	GENERATED_BODY()

public:
	
	AFFYRaiseAction();

	/*
	virtual FName GetLabel() const override
	{
		return "Raise";
	}

	virtual FText GetDescription() const override
	{
		return FText::FromString("Restore a party member from KO status with a small amount of HP.");
	}

	virtual uint8 GetType() const override
	{
		return 1;
	}

	virtual ETargetType GetTargetType() const override
	{
		return ETargetType::SINGLE;
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
		return 100;
	}

	virtual float GetHPCost() const override
	{
		return 0;
	}

	virtual float GetMPCost() const override
	{
		return 15;
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
	virtual void ExecuteAction(FBattleCharacterData& ActionOwner, FBattleCharacterData& Target) override;
	virtual void Redirect(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets) override;
	virtual void Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex) override;
};
