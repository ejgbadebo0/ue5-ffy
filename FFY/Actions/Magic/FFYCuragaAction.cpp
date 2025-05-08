// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYCuragaAction.h"

AFFYCuragaAction::AFFYCuragaAction()
{
	Label = "Curaga";
	Description = FText::FromString("Heal a target for a large amount of HP.");
	Type = 1;
	TargetType = ETargetType::BOTH;
	ActionType = EActionType::MAGIC;
	bIsDefaultTargetEnemy = false;
	bQueuedAction = true;
	bIsPassive = false;
	ATBCost = 100;
	HPCost = 0;
	MPCost = 20;
	LPCost = 0;
	Rank = 3;
}

float AFFYCuragaAction::GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner)
{
	return Super::GetCalculatedActionValue(ActionOwner);
}

void AFFYCuragaAction::ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	Super::ExecuteAction(ActionOwner, Targets);
}

void AFFYCuragaAction::ExecuteAction(FBattleCharacterData& ActionOwner,TArray<FBattleCharacterData>& Targets)
{
	Super::ExecuteAction(ActionOwner, Targets);
}

void AFFYCuragaAction::ExecuteAction(FBattleCharacterData& ActionOwner, FBattleCharacterData& Target)
{
	Super::ExecuteAction(ActionOwner, Target);
}

void AFFYCuragaAction::Redirect(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	Super::Redirect(ActionOwner, Targets);
}
