// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYCuraAction.h"

AFFYCuraAction::AFFYCuraAction()
{
	Label = "Cura";
	Description = FText::FromString("Heal a target for a moderate amount of HP.");
	Type = 1;
	TargetType = ETargetType::BOTH;
	ActionType = EActionType::MAGIC;
	bIsDefaultTargetEnemy = false;
	bQueuedAction = true;
	bIsPassive = false;
	ATBCost = 75;
	HPCost = 0;
	MPCost = 10;
	LPCost = 0;
	Rank = 2;
}

float AFFYCuraAction::GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner)
{
	return Super::GetCalculatedActionValue(ActionOwner);
}

void AFFYCuraAction::ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	Super::ExecuteAction(ActionOwner, Targets);
}

void AFFYCuraAction::ExecuteAction(FBattleCharacterData& ActionOwner, TArray<FBattleCharacterData>& Targets)
{
	Super::ExecuteAction(ActionOwner, Targets);
}

void AFFYCuraAction::ExecuteAction(FBattleCharacterData& ActionOwner, FBattleCharacterData& Target)
{
	Super::ExecuteAction(ActionOwner, Target);
}

void AFFYCuraAction::Redirect(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	Super::Redirect(ActionOwner, Targets);
}
