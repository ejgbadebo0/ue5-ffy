// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYFiraAction.h"

AFFYFiraAction::AFFYFiraAction()
{
	Label = "Fira";
	Description = FText::FromString("Deal a moderate amount of Fire damage to a target.");
	Type = 0;
	TargetType = ETargetType::BOTH;
	ActionType = EActionType::MAGIC;
	bIsDefaultTargetEnemy = true;
	bQueuedAction = true;
	bIsPassive = false;
	ATBCost = 75;
	HPCost = 0;
	MPCost = 10;
	LPCost = 0;
	Rank = 2;
}

float AFFYFiraAction::GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner)
{
	return Super::GetCalculatedActionValue(ActionOwner);
}

void AFFYFiraAction::ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	Super::ExecuteAction(ActionOwner, Targets);
}

void AFFYFiraAction::ExecuteAction(FBattleCharacterData& ActionOwner, TArray<FBattleCharacterData>& Targets)
{
	Super::ExecuteAction(ActionOwner, Targets);
}

void AFFYFiraAction::Redirect(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	Super::Redirect(ActionOwner, Targets);
}
