// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYItemAction.h"

#include "FFY/FFYItem.h"

AFFYItemAction::AFFYItemAction()
{
	Label = "Item";
	Description = FText::FromString("Use inventory items.");
	Type = 0;
	TargetType = ETargetType::SINGLE;
	ActionType = EActionType::ITEM;
	bIsDefaultTargetEnemy = false;
	bQueuedAction = false;
	bIsPassive = false;
	ATBCost = 100;
	HPCost = 0;
	MPCost = 0;
	LPCost = 0;
	Rank = 1;
}

void AFFYItemAction::ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	//Need to use owner to access GameInstance
	MenuLabel = ActionOwner->GetItemName_Implementation(UseItemID);
	
	Super::ExecuteAction(ActionOwner, Targets);
}

void AFFYItemAction::ExecuteAction(FBattleCharacterData& ActionOwner, TArray<FBattleCharacterData>& Targets)
{
	Super::ExecuteAction(ActionOwner, Targets);
}

void AFFYItemAction::Redirect(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	Super::Redirect(ActionOwner, Targets);
}

void AFFYItemAction::Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target)
{
	//can't access World object
	OnItemActionUse.Broadcast(UseItemID, ActionOwner, Target);
}
