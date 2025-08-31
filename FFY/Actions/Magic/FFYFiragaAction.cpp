// Source code implementation by Ephraim Gbadebo.


#include "FFYFiragaAction.h"

AFFYFiragaAction::AFFYFiragaAction()
{
	Label = "Firaga";
	Description = FText::FromString("Deal a large amount of Fire damage to a target.");
	Type = 0;
	TargetType = ETargetType::BOTH;
	ActionType = EActionType::MAGIC;
	bIsDefaultTargetEnemy = true;
	bQueuedAction = true;
	bIsPassive = false;
	ATBCost = 100;
	HPCost = 0;
	MPCost = 20;
	LPCost = 0;
	Rank = 3;
}

float AFFYFiragaAction::GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner)
{
	return Super::GetCalculatedActionValue(ActionOwner);
}

void AFFYFiragaAction::ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	Super::ExecuteAction(ActionOwner, Targets);
}

void AFFYFiragaAction::ExecuteAction(FBattleCharacterData& ActionOwner,TArray<FBattleCharacterData>& Targets)
{
	Super::ExecuteAction(ActionOwner, Targets);
}

void AFFYFiragaAction::Redirect(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	Super::Redirect(ActionOwner, Targets);
}
