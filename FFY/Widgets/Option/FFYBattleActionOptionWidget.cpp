// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYBattleActionOptionWidget.h"

#include "FFYPartyMemberOptionWidget.h"
#include "FFY/FFYAction.h"
#include "FFY/FFYBattleCharacter.h"
#include "FFY/Widgets/FFYMenuWidget.h"

void UFFYBattleActionOptionWidget::PerformAction()
{
	if ( bIsDisabled || OwnerMenu->GetCurrentMenuMode() != EMenuMode::NONE )
	{
		return;
	}
	if (Action)
	{
		OwnerMenu->SetDefaultTargetGroup_Implementation(Action->bIsDefaultTargetEnemy, false);
		OwnerMenu->StartSelection_Implementation(this, Action->TargetType);
	}

}

void UFFYBattleActionOptionWidget::OnSelected()
{
	Super::OnSelected();
	if (Action)
	{
		//Action->ActionContainer->BattleCharacter
		
		AFFYBattleCharacter* Character = OwnerMenu->GetOwnerMaster()->GetBattleContext_Implementation();
		if (Character)
		{
			Character->MenuActionSelected_Implementation(Action);
		}
	}
}

void UFFYBattleActionOptionWidget::OnUnselected()
{
	Super::OnUnselected();
	AFFYBattleCharacter* Character = OwnerMenu->GetOwnerMaster()->GetBattleContext_Implementation();
	if (Character)
	{
		Character->MenuActionSelected_Implementation(nullptr);
	}
}

void UFFYBattleActionOptionWidget::BattleContextAction_Implementation(AFFYBattleCharacter* Character, bool SelectAll)
{
	Super::BattleContextAction_Implementation(Character, SelectAll);
}

void UFFYBattleActionOptionWidget::ContextAction_Implementation(UFFYPartyMemberOptionWidget* CharacterWidget,
	bool SelectAll)
{
	AFFYBattleCharacter* User = OwnerMenu->GetOwnerMaster()->GetBattleContext_Implementation();
	AFFYBattleCharacter* Target = CharacterWidget->GetBattleCharacterReference();
	if (User && Target)
	{
		TArray<AFFYBattleCharacter*> Targets;
		
		if (SelectAll)
		{
			Targets = (Target->ActorHasTag(FName("Enemy"))) ? CharacterWidget->GetOwnerMenu()->GetTargetGroupActors_Implementation(true) : CharacterWidget->GetOwnerMenu()->GetTargetGroupActors_Implementation(false);
		}
		else
		{
			Targets.Emplace(Target);
		}
		
		Action->ExecuteAction(User, Targets);
		//GameInstance->ItemManager->Use(GameInstance->Inventory[Index], Party[i].PartyCharacterData);
		
		OwnerMenu->EndSelection_Implementation();
			//OwnerMenu->ContextRefresh_Implementation(UserIndex);
	}
}
