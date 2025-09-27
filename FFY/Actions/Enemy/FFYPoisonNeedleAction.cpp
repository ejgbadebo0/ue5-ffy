// Source code implementation by Ephraim Gbadebo.


#include "FFYPoisonNeedleAction.h"

#include "FFY/FFYBattleCharacter.h"

AFFYPoisonNeedleAction::AFFYPoisonNeedleAction()
{
	Label = "Poison Needle";
	Description = FText::FromString("An attack that has a small chance to inflict poison.");
	Type = 0;
	TargetType = ETargetType::SINGLE;
	ActionType = EActionType::NONE;
	bIsDefaultTargetEnemy = true;
	bQueuedAction = true;
	bIsPassive = false;
	ATBCost = 100;
	HPCost = 0;
	MPCost = 0;
	LPCost = 0;
	Rank = 1;
}

void AFFYPoisonNeedleAction::Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex)
{
	
	//Add a chance to inflict Death if Target HP is less than 10%
	TArray<EStatusEffect> Poison;
	Poison.Emplace(EStatusEffect::POISON);

	ActionOwner->UpdateDamageAttributes(
		GetCalculatedActionValue(ActionOwner),
		EStat::HP,
		EDamageModifier::PHYSICAL,
		EElement::STATUS,
		Poison,
		TArray<EStatusEffect>(),
		0.2f,
		EAttackType::MELEE);

	//Deal Damage
	ActionOwner->DamageDealtEvent(Target->ReceiveDamage_Implementation(ActionOwner, this));
}
