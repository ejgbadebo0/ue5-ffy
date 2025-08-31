// Source code implementation by Ephraim Gbadebo.


#include "FFYFireAction.h"

#include "FFY/FFYBattleCharacter.h"

AFFYFireAction::AFFYFireAction()
{
	Label = "Fire";
	Description = FText::FromString("Deal a small amount of Fire damage to a target.");
	Type = 0;
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

float AFFYFireAction::GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner)
{
	
	return FMath::Floor(Rank * ((FMath::Square(ActionOwner->BattleCharacterStats.Magic))/6 + Rank)/4 );
}

void AFFYFireAction::ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	ActionOwner->UpdateDamageAttributes(
		GetCalculatedActionValue(ActionOwner),
		EStat::HP,
		EDamageModifier::MAGIC,
		EElement::FIRE,
	TArray<EStatusEffect>(),
	TArray<EStatusEffect>(),
		EAttackType::RANGED);
	
	Super::ExecuteAction(ActionOwner, Targets);
}

void AFFYFireAction::ExecuteAction(FBattleCharacterData& ActionOwner, TArray<FBattleCharacterData>& Targets)
{
	Super::ExecuteAction(ActionOwner, Targets);
}

void AFFYFireAction::Redirect(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	Super::Redirect(ActionOwner, Targets);
}

void AFFYFireAction::Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex)
{
	ActionOwner->UpdateDamageAttributes(
	GetCalculatedActionValue(ActionOwner),
	EStat::HP,
	EDamageModifier::MAGIC,
	EElement::FIRE,
TArray<EStatusEffect>(),
TArray<EStatusEffect>(),
	EAttackType::RANGED);

	Target->ReceiveDamage_Implementation(ActionOwner, this);
}
