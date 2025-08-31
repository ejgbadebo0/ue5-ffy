// Source code implementation by Ephraim Gbadebo.


#include "FFYAdvanceAction.h"

AFFYAdvanceAction::AFFYAdvanceAction()
{
	Label = "Advance";
	Description = FText::FromString("Attack with a follow-up strike.");
	Type = 0;
	TargetType = ETargetType::SINGLE;
	ActionType = EActionType::NONE;
	bIsDefaultTargetEnemy = true;
	bQueuedAction = false;
	bIsPassive = false;
	ATBCost = 25;
	HPCost = 0;
	MPCost = 0;
	LPCost = 0;
	Rank = 1;
}

float AFFYAdvanceAction::GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner)
{
	return FMath::Floor(Super::GetCalculatedActionValue(ActionOwner) * FMath::RandRange(1.f, 1.5f));
}
