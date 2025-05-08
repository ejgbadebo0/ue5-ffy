// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FFYSpellAction.h"
#include "FFYCureAction.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API AFFYCureAction : public AFFYSpellAction
{
	GENERATED_BODY()

public:
	AFFYCureAction();

	/*
	virtual FName GetLabel() const override
	{
		return "Cure";
	}

	virtual uint8 GetType() const override
	{
		return 1;
	}

	virtual FText GetDescription() const override
	{
		return FText::FromName("Heal a target for a small amount of HP.");
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
		return Super::GetRank();
	}
	*/

	virtual float GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner) override;
	virtual void ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets) override;
	virtual void ExecuteAction(FBattleCharacterData& ActionOwner, TArray<FBattleCharacterData>& Targets) override;
	virtual void ExecuteAction(FBattleCharacterData& ActionOwner, FBattleCharacterData& Target) override;
	virtual void Redirect(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets) override;
	virtual void Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target) override;
};
