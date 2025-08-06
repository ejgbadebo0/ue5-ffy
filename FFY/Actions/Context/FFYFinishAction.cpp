// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYFinishAction.h"

#include "FFY/FFYBattleCharacter.h"


AFFYFinishAction::AFFYFinishAction()
{
	Label = "Finish";
	Description = FText::FromString("Attack with a powerful ending strike that has a chance to insta-kill enemies low on HP.");
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

float AFFYFinishAction::GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner)
{
	return FMath::Floor(Super::GetCalculatedActionValue(ActionOwner) * FMath::RandRange(1.5f, 2.f));
}

void AFFYFinishAction::Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target)
{
	EAttackType InAttackType = (ActionOwner->BattleCharacterStats.Equipment.Weapon.bIsEquipable) ? ActionOwner->BattleCharacterStats.Equipment.Weapon.EquipmentData.AttackType : EAttackType::MELEE;
	EDamageModifier InDamageModifier = (ActionOwner->BattleCharacterStats.Equipment.Weapon.bIsEquipable) ? ActionOwner->BattleCharacterStats.Equipment.Weapon.EquipmentData.DamageModifier : EDamageModifier::PHYSICAL;

	//Add a chance to inflict Death if Target HP is less than 10%
	TArray<EStatusEffect> Death;
	Death.Emplace(EStatusEffect::DEATH);
	
	TArray<EStatusEffect> Inflict = ((Target->BattleCharacterStats.HP/Target->BattleCharacterStats.MaxHP) <= 0.1f) ?  Death : TArray<EStatusEffect>();

	ActionOwner->UpdateDamageAttributes(
		GetCalculatedActionValue(ActionOwner),
		EStat::HP,
		InDamageModifier,
		ActionOwner->Infusion,
		Inflict,
		TArray<EStatusEffect>(),
		InAttackType);

	//Deal Damage
	ActionOwner->DamageDealtEvent(Target->ReceiveDamage_Implementation(ActionOwner, this));
}
