// Source code implementation by Ephraim Gbadebo.


#include "FFYBarrierAction.h"

#include "FFY/FFYBattleCharacter.h"

AFFYBarrierAction::AFFYBarrierAction()
{
	Label = "Barrier";
	Description = FText::FromString("Create a shield that blocks and repels Physical attacks for a small duration.");
	Type = 0;
	TargetType = ETargetType::BOTH;
	ActionType = EActionType::MAGIC;
	bIsDefaultTargetEnemy = false;
	bQueuedAction = true;
	bIsPassive = false;
	ATBCost = 25;
	HPCost = 0;
	MPCost = 10;
	LPCost = 0;
	Rank = 2;
}

void AFFYBarrierAction::Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex)
{
	switch (HitIndex)
	{
	case 0:
			Target->BattleCharacterStats.StatusEffects.Emplace(EStatusEffect::BARRIER);
			break;
	case 1:
			Target->BattleCharacterStats.StatusEffects.Remove(EStatusEffect::BARRIER);
	}
}
