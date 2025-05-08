// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Actions/Magic/FFYFireAction.h"
#include "FFYFiragaAction.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API AFFYFiragaAction : public AFFYFireAction
{
	GENERATED_BODY()

public:
	AFFYFiragaAction();

	/*
	virtual FName GetLabel() const override
	{
		return "Firaga";
	}

	virtual FText GetDescription() const override
	{
		return FText::FromString("Deal a large amount of Fire damage to a target.");
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
	}
	*/
	
	virtual float GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner) override;
	virtual void ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets) override;
	virtual void ExecuteAction(FBattleCharacterData& ActionOwner, TArray<FBattleCharacterData>& Targets) override;
	virtual void Redirect(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets) override;
};
