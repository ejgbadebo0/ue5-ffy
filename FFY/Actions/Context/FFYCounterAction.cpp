// Source code implementation by Ephraim Gbadebo.


#include "FFYCounterAction.h"

#include "FFY/FFYBattleCharacter.h"

AFFYCounterAction::AFFYCounterAction()
{
	Label = "Counter";
	Description = FText::FromString("After a successful Parry, follow up with a counter attack.");
	Type = 0;
	TargetType = ETargetType::SINGLE;
	ActionType = EActionType::NONE;
	bIsDefaultTargetEnemy = true;
	bQueuedAction = false;
	bIsPassive = false;
	ATBCost = 50;
	HPCost = 0;
	MPCost = 0;
	LPCost = 0;
	Rank = 1;
}

float AFFYCounterAction::GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner)
{
	float BaseStat = 1.f;
	switch (ActionOwner->BattleCharacterStats.Equipment.Weapon.EquipmentData.DamageModifier)
	{
		case EDamageModifier::NONE:
			break;
		case EDamageModifier::PHYSICAL:
			BaseStat = ActionOwner->BattleCharacterStats.Strength;
			break;
		case EDamageModifier::MAGIC:
			BaseStat = ActionOwner->BattleCharacterStats.Magic * 1.25;
			break;
	}
	return FMath::Floor(((BaseStat/2) + ActionOwner->BattleCharacterStats.LV) * FMath::RandRange(1.f, 1.35f));
}

void AFFYCounterAction::ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	TArray<AFFYBattleCharacter*> Target;
	if (ActionOwner->DamageSource != nullptr)
	{
		Target.Add(ActionOwner->DamageSource);
		Super::ExecuteAction(ActionOwner, Target);
	}
	
}
