// Source code implementation by Ephraim Gbadebo.


#include "FFYTailSwipeAction.h"

#include "FFY/FFYBattleCharacter.h"

AFFYTailSwipeAction::AFFYTailSwipeAction()
{
	Label = "Tail Swipe";
	Description = FText::FromString("Hit all targets simultaneously.");
	Type = 0;
	TargetType = ETargetType::MULTI;
	ActionType = EActionType::NONE;
	bIsDefaultTargetEnemy = true;
	bQueuedAction = false;
	bIsPassive = false;
	ATBCost = 100;
	HPCost = 0;
	MPCost = 0;
	LPCost = 0;
	Rank = 1;
}

float AFFYTailSwipeAction::GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner)
{
	return Super::GetCalculatedActionValue(ActionOwner) * FMath::RandRange(0.55f, 0.7f);
}

void AFFYTailSwipeAction::Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex)
{
	ActionOwner->UpdateDamageAttributes(
		GetCalculatedActionValue(ActionOwner),
		EStat::HP,
		EDamageModifier::PHYSICAL,
		ActionOwner->Infusion,
		TArray<EStatusEffect>(),
		TArray<EStatusEffect>(),
		0.0f,
		EAttackType::MELEE);

	//Deal Damage to all targets
	for (auto T : ActionOwner->Targets)
	{
		ActionOwner->DamageDealtEvent(T->ReceiveDamage_Implementation(ActionOwner, this));
	}
	
}
