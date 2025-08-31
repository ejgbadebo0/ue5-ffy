// Source code implementation by Ephraim Gbadebo.


#include "FFYCureAction.h"
#include "FFY/FFYBattleCharacter.h"

AFFYCureAction::AFFYCureAction()
{
	Label = "Cure";
	Description = FText::FromName("Heal a target for a small amount of HP.");
	Type = 1;
	TargetType = ETargetType::BOTH;
	ActionType = EActionType::MAGIC;
	bIsDefaultTargetEnemy = false;
	bQueuedAction = true;
	bIsPassive = false;
	ATBCost = 50;
	HPCost = 0;
	MPCost = 5;
	LPCost = 0;
	Rank = 1;
}

float AFFYCureAction::GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner)
{
	return FMath::Floor((Rank * (ActionOwner->BattleCharacterStats.Spirit + Rank/2)) );
}

void AFFYCureAction::ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	ActionOwner->UpdateDamageAttributes(
	GetCalculatedActionValue(ActionOwner),
		EStat::HP,
		EDamageModifier::MAGIC,
		EElement::HEALING,
	TArray<EStatusEffect>(),
	TArray<EStatusEffect>(),
		EAttackType::RANGED);

	Super::ExecuteAction(ActionOwner, Targets);
}

void AFFYCureAction::ExecuteAction(FBattleCharacterData& ActionOwner, TArray<FBattleCharacterData>& Targets)
{
	Super::ExecuteAction(ActionOwner, Targets);
}

void AFFYCureAction::ExecuteAction(FBattleCharacterData& ActionOwner, FBattleCharacterData& Target)
{
	if (CanCast(ActionOwner))
	{
		bool Result = (Target.HP < Target.MaxHP);
		if (Result)
		{
			float Value = FMath::Floor((Rank * (ActionOwner.Spirit + Rank/2)) );
			Target.HP = FMath::Min(Target.HP + Value, Target.MaxHP);
			ActionOwner.MP -= MPCost;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("HPREC: %f"), Value ));
		}
	}
}

void AFFYCureAction::Redirect(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	Super::Redirect(ActionOwner, Targets);
}

void AFFYCureAction::Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex)
{
	ActionOwner->UpdateDamageAttributes(
GetCalculatedActionValue(ActionOwner),
	EStat::HP,
	EDamageModifier::MAGIC,
	EElement::HEALING,
TArray<EStatusEffect>(),
TArray<EStatusEffect>(),
	EAttackType::RANGED);

	Target->ReceiveHealing_Implementation(ActionOwner, this);
}
