// Source code implementation by Ephraim Gbadebo.


#include "FFYRaiseAction.h"
#include "FFY/FFYBattleCharacter.h"

AFFYRaiseAction::AFFYRaiseAction()
{
	Label = "Raise";
	Description = FText::FromString("Restore a party member from KO status with a small amount of HP.");
	Type = 1;
	TargetType = ETargetType::SINGLE;
	ActionType = EActionType::MAGIC;
	bIsDefaultTargetEnemy = false;
	bQueuedAction = true;
	bIsPassive = false;
	ATBCost = 100;
	HPCost = 0;
	MPCost = 15;
	LPCost = 0;
	Rank = 1;
}

float AFFYRaiseAction::GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner)
{
	return Super::GetCalculatedActionValue(ActionOwner);
}

void AFFYRaiseAction::ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	TArray<EStatusEffect> Effects;
	Effects.Emplace(EStatusEffect::KO);
	
	ActionOwner->UpdateDamageAttributes(
		(Targets[0]->BattleCharacterStats.MaxHP * 0.05f),
		EStat::HP,
		EDamageModifier::MAGIC,
		EElement::STATUS,
		TArray<EStatusEffect>(),
		Effects,
		EAttackType::RANGED);

	Super::ExecuteAction(ActionOwner, Targets);
}

void AFFYRaiseAction::ExecuteAction(FBattleCharacterData& ActionOwner, TArray<FBattleCharacterData>& Targets)
{
	Super::ExecuteAction(ActionOwner, Targets);
}

void AFFYRaiseAction::ExecuteAction(FBattleCharacterData& ActionOwner, FBattleCharacterData& Target)
{
	if (CanCast(ActionOwner))
	{
		bool Result = (Target.StatusEffects.Contains(EStatusEffect::KO));
		if (Result)
		{
			float Value = FMath::Floor( Target.MaxHP * 0.05f );
			Target.StatusEffects.RemoveSingle(EStatusEffect::KO);
			Target.HP = FMath::Min(Target.HP + Value, Target.MaxHP);
			ActionOwner.MP -= MPCost;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("HPREC: %f"), Value ));
		}
	}
}

void AFFYRaiseAction::Redirect(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	Super::Redirect(ActionOwner, Targets);
}

void AFFYRaiseAction::Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex)
{
	TArray<EStatusEffect> Effects;
	Effects.Emplace(EStatusEffect::KO);
	float HPValue = Target->BattleCharacterStats.MaxHP;
	float RevRestoreAmount = HPValue * 0.05f;
	
	ActionOwner->UpdateDamageAttributes(
	RevRestoreAmount,
	EStat::HP,
	EDamageModifier::MAGIC,
	EElement::STATUS,
	TArray<EStatusEffect>(),
	Effects,
	EAttackType::RANGED);

	Target->ReceiveHealing_Implementation(ActionOwner, this);

}
