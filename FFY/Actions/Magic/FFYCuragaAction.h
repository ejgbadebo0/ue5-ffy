// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Actions/Magic/FFYCureAction.h"
#include "FFYCuragaAction.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API AFFYCuragaAction : public AFFYCureAction
{
	GENERATED_BODY()

public:
	AFFYCuragaAction();

	/*
	virtual FName GetLabel() const override
	{
		return "Curaga";
	}

	virtual FText GetDescription() const override
	{
		return FText::FromString("Heal a target for a large amount of HP.");
	}

	virtual uint8 GetType() const override
	{
		return Super::GetType();
	}

	virtual ETargetType GetTargetType() const override
	{
		return Super::GetTargetType();
	}

	virtual EActionType GetActionType() const override
	{
		return Super::GetActionType();
	}

	virtual bool GetIsQueuedAction() const override
	{
		return Super::GetIsQueuedAction();
	}

	virtual float GetATBCost() const override
	{
		return 100;
	}

	virtual float GetHPCost() const override
	{
		return Super::GetHPCost();
	}

	virtual float GetMPCost() const override
	{
		return 20;
	}

	virtual float GetLPCost() const override
	{
		return Super::GetLPCost();
	}

	virtual float GetRank() const override
	{
		return 3;
	}*/

	virtual float GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner) override;
	virtual void ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets) override;
	virtual void ExecuteAction(FBattleCharacterData& ActionOwner, TArray<FBattleCharacterData>& Targets) override;
	virtual void ExecuteAction(FBattleCharacterData& ActionOwner, FBattleCharacterData& Target) override;
	virtual void Redirect(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets) override;
};
