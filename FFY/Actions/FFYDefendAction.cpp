// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYDefendAction.h"

#include "FFY/FFYBattleCharacter.h"

AFFYDefendAction::AFFYDefendAction()
{
	Label = "Defend";
	Description = FText::FromString("Negate physical damage from attacks.");
	Type = 0;
	TargetType = ETargetType::NONE;
	ActionType = EActionType::STATE;
	bIsDefaultTargetEnemy = false;
	bQueuedAction = false;
	bIsPassive = false;
	ATBCost = -1;
	HPCost = 0;
	MPCost = 0;
	LPCost = 0;
	Rank = 1;
}

void AFFYDefendAction::ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	ActionOwner->ActionState = EActionState::DEFENDING;
}

void AFFYDefendAction::ExecuteAction(FBattleCharacterData& ActionOwner, TArray<FBattleCharacterData>& Targets)
{
	Super::ExecuteAction(ActionOwner, Targets);
}

void AFFYDefendAction::Redirect(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	Super::Redirect(ActionOwner, Targets);
}
