// Source code implementation by Ephraim Gbadebo.


#include "FFYAction.h"
//ext includes
#include "FFYActionContainer.h"
#include "FFYBattleCharacter.h"
#include "FFYBattleCharacterAnimInstance.h"
#include "AI/NavigationSystemBase.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"


float AFFYAction::GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner)
{
	return 0;
}

float AFFYAction::GetVariableATBCost(AFFYBattleCharacter* ActionOwner)
{
	return  FMath::Max(1, ActionOwner->ATB);
}

FName AFFYAction::GetMenuLabel()
{
	return (MenuLabel != NAME_None) ? MenuLabel : Label;
}

void AFFYAction::ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	//can use for free
	if (ActionOwner->bIsFocused)
	{
		FreeUse(ActionOwner, Targets);
		return;
	}
	//has the resources and not inhibited by status
	if (CanUse(ActionOwner, Targets.Num()))
	{
		
		if (!CanExecute(ActionOwner) || (ActionOwner->ActionState == EActionState::ACTING) || (ActionOwner->ActiveState == EActiveState::WAIT)) //if not currently in a state where character can expend ATB
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("QUEUE ACTION ATTEMPT"));
			if (bQueuedAction) //if this is an action we can queue 
			{
				ActionOwner->QueueAction(this, Targets);
			}
			return;
		}
		Consume(ActionOwner, Targets.Num());

		//Set variables for delayed effects separate from animation
		ActionOwner->ActionContainer->LastAction = this;
		ActionOwner->Targets = Targets;

		//Debug stuff ======
		FString Ostr = ActionOwner->BattleCharacterStats.CharacterName.ToString();
		FString Astr = Label.ToString();
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Action %s, by: %s"), *Astr, *Ostr ));

		for (auto i : Targets) {
			if (i)
			{
				FString Tstr = i->BattleCharacterStats.CharacterName.ToString();
			}
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Performed on: %s"), *Tstr));
		}
		//===========
		//reset to Idle in case of Defending
		if (ActionOwner->ActionState == EActionState::DEFENDING && ActionType != EActionType::STATE)  
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

			ActionOwner->ActionUsed_Implementation(GetMenuLabel(), ActionOwner->ActorHasTag(FName("Enemy")));
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

void AFFYAction::Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex)
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
		//is in a condition to use
		for (auto e : ActionOwner->BattleCharacterStats.StatusEffects)
		{
			FAffectedActionTypes* Result = ActionOwner->ActionDisablingEffects.Find(e);
			if (Result != nullptr)
			{
				if (Result->ActionTypes.Contains(ActionType))
				{
					return false;
				}
			}
		}
		
		//has the resources to use, and not queueing another attack
		bool StateEval = ActionOwner->ActionState != EActionState::CASTING;
		bool HPEval = (TargetType == ETargetType::BOTH) ? (ActionOwner->BattleCharacterStats.HP >= HPCost * Targets) : (ActionOwner->BattleCharacterStats.HP >= HPCost);
		bool MPEval = (TargetType == ETargetType::BOTH) ? (ActionOwner->BattleCharacterStats.MP >= MPCost * Targets) : (ActionOwner->BattleCharacterStats.MP >= MPCost);

		return (StateEval) && (MPEval) && (HPEval);
	}
}

bool AFFYAction::CanExecute(AFFYBattleCharacter* ActionOwner)
{
	bool ATBEval = (ATBCost <= 0) ? (ActionOwner->ATB >= GetVariableATBCost(ActionOwner)) : (ActionOwner->ATB >= ATBCost);
		
	return (ATBEval); 
}

void AFFYAction::Consume(AFFYBattleCharacter* ActionOwner, int8 Targets)
{
	float ATBConsume = (ATBCost <= 0) ? (GetVariableATBCost(ActionOwner)) : (ATBCost);
	float HPConsume = (TargetType == ETargetType::BOTH) ? HPCost * Targets : HPCost;
	float MPConsume = (TargetType == ETargetType::BOTH) ? MPCost * Targets : MPCost;

	ActionOwner->Consume(ATBConsume, HPConsume, MPConsume);
}

void AFFYAction::FreeUse(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	//Set variables for delayed effects separate from animation
	ActionOwner->ActionContainer->LastAction = this;
	ActionOwner->Targets = Targets;

	//reset to Idle in case of Defending
	if (ActionOwner->ActionState == EActionState::DEFENDING && ActionType != EActionType::STATE)  
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

		ActionOwner->ActionUsed_Implementation(GetMenuLabel(), ActionOwner->ActorHasTag(FName("Enemy")));
	}
	//having used a free action
	ActionOwner->OnFreeActionUse();
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
