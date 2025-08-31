// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/FFYAction.h"
#include "FFYMagicAction.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API AFFYMagicAction : public AFFYAction
{
	GENERATED_BODY()

public:
	AFFYMagicAction();
	/*
	virtual FName GetLabel() const override
	{
		return "Magic";
	}

	virtual FText GetDescription() const override
	{
		return FText::FromString("Cast magic abilities.");
	}

	virtual uint8 GetType() const override
	{
		return 1;
	}

	virtual ETargetType GetTargetType() const override
	{
		return Super::GetTargetType();
	}

	virtual EActionType GetActionType() const override
	{
		return EActionType::MENU;
	}

	virtual bool GetIsQueuedAction() const override
	{
		return false;
	}

	virtual float GetATBCost() const override
	{
		return 0;
	}

	virtual float GetHPCost() const override
	{
		return 0;
	}

	virtual float GetMPCost() const override
	{
		return 0;
	}

	virtual float GetLPCost() const override
	{
		return 0;
	}

	virtual float GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner) override
	{
		return 0;
	}

	virtual float GetRank() const override
	{
		return Super::GetRank();
	}
	*/

	virtual void ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets) override;
	virtual void ExecuteAction(FBattleCharacterData& ActionOwner, TArray<FBattleCharacterData>& Targets) override;
	virtual void Redirect(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets) override;
};
