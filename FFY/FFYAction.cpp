// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYAction.h"
//ext includes
#include "FFYActionContainer.h"
#include "FFYBattleCharacter.h"
#include "FFYBattleCharacterAnimInstance.h"


float AFFYAction::GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner)
{
	return 0;
}

float AFFYAction::GetVariableATBCost(AFFYBattleCharacter* ActionOwner)
{
	return  FMath::Max(1, ActionOwner->ATB);
}

void AFFYAction::ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	if (CanUse(ActionOwner, Targets.Num()))
	{
		Consume(ActionOwner, Targets.Num());

		//Set variables for delayed effects separate from animation
		ActionOwner->ActionContainer->LastAction = this;
		ActionOwner->Targets = Targets;

		//Debug stuff ======
		FString Ostr = ActionOwner->BattleCharacterStats.CharacterName.ToString();
		FString Astr = Label.ToString();
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Action %s, by: %s"), *Astr, *Ostr ));

		for (auto i : Targets) {
			FString Tstr = i->BattleCharacterStats.CharacterName.ToString();
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Performed on: %s"), *Tstr));
		}
		//===========
		//reset to Idle in case of Defending
		if (ActionOwner)  
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Silver, "ACTION OWNER VALID");
			ActionOwner->ActionState = EActionState::IDLE;
		}
		//Play animation tied to this action
		IFFYAnimationControls* AnimInstance = Cast<IFFYAnimationControls>(ActionOwner->AnimInstance);
		
		if (AnimInstance != nullptr)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Silver, "ANIM INSTANCE VALID: Play Montage");
			AnimInstance->PlayActionMontage_Implementation(Label, (Targets.Num() > 1));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Silver, "FAILED TO EXECUTE: Resources");
	}
}

void AFFYAction::ExecuteAction(FBattleCharacterData& ActionOwner, TArray<FBattleCharacterData>& Targets)
{
	FString Ostr = ActionOwner.CharacterName.ToString();
	FString Astr = Label.ToString();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Action %s, by: %s"), *Astr, *Ostr));
	FString Tstr = Targets[0].CharacterName.ToString();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Performed on: %s"), *Tstr));

}

void AFFYAction::ExecuteAction(FBattleCharacterData& ActionOwner, FBattleCharacterData& Target)
{
}

void AFFYAction::Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target)
{
	return;
}

bool AFFYAction::CanCast(FBattleCharacterData& ActionOwner)
{
	return (ActionOwner.MP >= MPCost &&
			!ActionOwner.StatusEffects.Contains(EStatusEffect::KO) &&
			!ActionOwner.StatusEffects.Contains(EStatusEffect::SILENCE)) &&
				!ActionOwner.StatusEffects.Contains(EStatusEffect::PARALYSIS) &&
					!ActionOwner.StatusEffects.Contains(EStatusEffect::SLEEP) &&
						!ActionOwner.StatusEffects.Contains(EStatusEffect::STOP);
}

bool AFFYAction::CanUse(AFFYBattleCharacter* ActionOwner, int8 Targets)
{
	if (ActionOwner == nullptr)
	{
		return false;
	}
	else
	{
		bool ATBEval = (ATBCost <= 0) ? (ActionOwner->ATB >= GetVariableATBCost(ActionOwner)) : (ActionOwner->ATB >= ATBCost);
		bool HPEval = (TargetType == ETargetType::BOTH) ? (ActionOwner->BattleCharacterStats.HP >= HPCost * Targets) : (ActionOwner->BattleCharacterStats.HP >= HPCost);
		bool MPEval = (TargetType == ETargetType::BOTH) ? (ActionOwner->BattleCharacterStats.MP >= MPCost * Targets) : (ActionOwner->BattleCharacterStats.MP >= MPCost);

		return (ATBEval) && (MPEval) && (HPEval);
	}
}

void AFFYAction::Consume(AFFYBattleCharacter* ActionOwner, int8 Targets)
{
	float ATBConsume = (ATBCost <= 0) ? (GetVariableATBCost(ActionOwner)) : (ATBCost);
	float HPConsume = (TargetType == ETargetType::BOTH) ? HPCost * Targets : HPCost;
	float MPConsume = (TargetType == ETargetType::BOTH) ? MPCost * Targets : MPCost;

	ActionOwner->Consume(ATBConsume, HPConsume, MPConsume);
}

void AFFYAction::DebugLocations(FVector Location1, FVector Normal1, FVector Location2, FVector Normal2)
{
	return;
}

void AFFYAction::Redirect(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	FString Ostr = ActionOwner->BattleCharacterStats.CharacterName.ToString();
	FString Astr = Label.ToString();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Redirect %s for: % s"), *Astr, *Ostr));
}
