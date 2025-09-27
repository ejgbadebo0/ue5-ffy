// Source code implementation by Ephraim Gbadebo.


#include "FFYBioAction.h"

#include "FFY/FFYBattleCharacter.h"

AFFYBioAction::AFFYBioAction()
{
	Label = "Bio";
	Description = FText::FromString("Deal a small amount of non-elemental Poison magic damage.");
	Type = 0;
	TargetType = ETargetType::BOTH;
	ActionType = EActionType::MAGIC;
	bIsDefaultTargetEnemy = true;
	bQueuedAction = true;
	bIsPassive = false;
	ATBCost = 75;
	HPCost = 0;
	MPCost = 10;
	LPCost = 0;
	Rank = 2;
}

float AFFYBioAction::GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner)
{
	return FMath::Floor(Rank * ((FMath::Square(ActionOwner->BattleCharacterStats.Magic))/6 + Rank)/4 );
}

void AFFYBioAction::Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex)
{

	TArray<EStatusEffect> Poison;
	Poison.Emplace(EStatusEffect::POISON);
	
	ActionOwner->UpdateDamageAttributes(
	GetCalculatedActionValue(ActionOwner),
	EStat::HP,
	EDamageModifier::MAGIC,
	EElement::NONE,
	Poison,
	TArray<EStatusEffect>(),
	0.0f,
	EAttackType::RANGED);

	Target->ReceiveDamage_Implementation(ActionOwner, this);
}
