// Source code implementation by Ephraim Gbadebo.


#include "FFYBlizzardAction.h"

#include "FFY/FFYBattleCharacter.h"

AFFYBlizzardAction::AFFYBlizzardAction()
{
	Label = "Blizzard";
	Description = FText::FromString("Deal a small amount of Ice damage to a target.");
	Type = 3;
	TargetType = ETargetType::BOTH;
	ActionType = EActionType::MAGIC;
	bIsDefaultTargetEnemy = true;
	bQueuedAction = true;
	bIsPassive = false;
	ATBCost = 50;
	HPCost = 0;
	MPCost = 5;
	LPCost = 0;
	Rank = 1;
}

float AFFYBlizzardAction::GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner)
{
	return FMath::Floor(Rank * ((FMath::Square(ActionOwner->BattleCharacterStats.Magic))/6 + Rank)/4 );
}

void AFFYBlizzardAction::Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex)
{
	ActionOwner->UpdateDamageAttributes(
	GetCalculatedActionValue(ActionOwner),
	EStat::HP,
	EDamageModifier::MAGIC,
	EElement::ICE,
	TArray<EStatusEffect>(),
	TArray<EStatusEffect>(),
	0.0f,
	EAttackType::RANGED);

	Target->ReceiveDamage_Implementation(ActionOwner, this);
}

