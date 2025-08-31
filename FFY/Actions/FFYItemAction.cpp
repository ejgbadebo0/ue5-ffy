// Source code implementation by Ephraim Gbadebo.


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
	bQueuedAction = true;
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

void AFFYItemAction::Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex)
{
	//can't access World object
	OnItemActionUse.Broadcast(UseItemID, ActionOwner, Target);
}
