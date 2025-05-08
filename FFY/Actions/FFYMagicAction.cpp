// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYMagicAction.h"

AFFYMagicAction::AFFYMagicAction()
{
	Label = "Magic";
	Description = FText::FromString("Cast magic abilities.");
	Type = 0;
	TargetType = ETargetType::NONE;
	ActionType = EActionType::MENU;
	bIsDefaultTargetEnemy = true;
	bQueuedAction = false;
	bIsPassive = false;
	ATBCost = 0;
	HPCost = 0;
	MPCost = 0;
	LPCost = 0;
	Rank = 1;
}

void AFFYMagicAction::ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	Super::ExecuteAction(ActionOwner, Targets);
}

void AFFYMagicAction::ExecuteAction(FBattleCharacterData& ActionOwner, TArray<FBattleCharacterData>& Targets)
{
	Super::ExecuteAction(ActionOwner, Targets);
}

void AFFYMagicAction::Redirect(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	Super::Redirect(ActionOwner, Targets);
}
