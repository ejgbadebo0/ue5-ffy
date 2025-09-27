// Source code implementation by Ephraim Gbadebo.


#include "FFYEsunaAction.h"

#include "FFY/FFYBattleCharacter.h"

AFFYEsunaAction::AFFYEsunaAction()
{
	Label = "Esuna";
	Description = FText::FromName("Remove common negative status effects from a target.");
	Type = 1;
	TargetType = ETargetType::SINGLE;
	ActionType = EActionType::MAGIC;
	bIsDefaultTargetEnemy = false;
	bQueuedAction = true;
	bIsPassive = false;
	ATBCost = 50;
	HPCost = 0;
	MPCost = 10;
	LPCost = 0;
	Rank = 1;
}

void AFFYEsunaAction::Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex)
{

	TArray<EStatusEffect> RemoveableEffects = {
		EStatusEffect::POISON,
		EStatusEffect::BLIND,
		EStatusEffect::PARALYSIS,
		EStatusEffect::MINI,
		EStatusEffect::SILENCE,
		EStatusEffect::CONFUSE,
		EStatusEffect::PETRIFY,
		EStatusEffect::SLEEP,
		EStatusEffect::BERSERK
		};
	
	ActionOwner->UpdateDamageAttributes(
	0.0f,
	EStat::HP,
	EDamageModifier::MAGIC,
	EElement::STATUS,
	TArray<EStatusEffect>(),
	RemoveableEffects,
	0.0f,
	EAttackType::RANGED);

	Target->ReceiveHealing_Implementation(ActionOwner, this);
}

void AFFYEsunaAction::ExecuteAction(FBattleCharacterData& ActionOwner, FBattleCharacterData& Target)
{
	if (CanCast(ActionOwner))
	{
		TArray<EStatusEffect> RemoveableEffects = {
			EStatusEffect::POISON,
			EStatusEffect::BLIND,
			EStatusEffect::PARALYSIS,
			EStatusEffect::MINI,
			EStatusEffect::SILENCE,
			EStatusEffect::CONFUSE,
			EStatusEffect::PETRIFY,
			EStatusEffect::SLEEP,
			EStatusEffect::BERSERK
			};
		
		bool Result = false;

		for (EStatusEffect Effect : RemoveableEffects)
		{
			if (Target.StatusEffects.Contains(Effect))
			{
				Target.StatusEffects.Remove(Effect);
				Result = true;
			}
		}
		
		if (Result)
		{
			ActionOwner.MP -= MPCost;
		}
	}
}
