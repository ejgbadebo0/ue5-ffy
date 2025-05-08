// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYBattleItemOptionWidget.h"

#include "FFYPartyMemberOptionWidget.h"
#include "FFY/FFYActionContainer.h"
#include "FFY/FFYBattleCharacter.h"
#include "FFY/Actions/FFYItemAction.h"
#include "FFY/Widgets/FFYMenuWidget.h"

void UFFYBattleItemOptionWidget::OnSelected()
{
	Super::OnSelected();
	AFFYBattleCharacter* Character = OwnerMenu->GetOwnerMaster()->GetBattleContext_Implementation();
	if (Character)
	{
		AFFYItemAction* ItemAction = OwnerMenu->GetOwnerMaster()->GetBattleContext_Implementation()->ActionContainer->Item;
		if (ItemAction)
		{
			Character->MenuActionSelected_Implementation(ItemAction);
		}
	}
}

void UFFYBattleItemOptionWidget::OnUnselected()
{
	Super::OnUnselected();
	AFFYBattleCharacter* Character = OwnerMenu->GetOwnerMaster()->GetBattleContext_Implementation();
	if (Character)
	{
		Character->MenuActionSelected_Implementation(nullptr);
	}
}

void UFFYBattleItemOptionWidget::PerformAction()
{
	if ( bIsDisabled || OwnerMenu->GetCurrentMenuMode() != EMenuMode::NONE )
	{
		return;
	}

	OwnerMenu->SetDefaultTargetGroup_Implementation(false, false);
	OwnerMenu->StartSelection_Implementation(this, ETargetType::SINGLE);
}

void UFFYBattleItemOptionWidget::ContextAction_Implementation(UFFYPartyMemberOptionWidget* CharacterWidget,
                                                              bool SelectAll)
{
	AFFYBattleCharacter* User = OwnerMenu->GetOwnerMaster()->GetBattleContext_Implementation();
	AFFYBattleCharacter* Target = CharacterWidget->GetBattleCharacterReference();
	if (User && Target)
	{
		if (User->ActionContainer->Item)
		{
			TArray<AFFYBattleCharacter*> Targets;
			Targets.Add(Target);
			User->ActionContainer->Item->UseItemID = InventoryItem.ID;
			User->ActionContainer->Item->ExecuteAction(User, Targets);

			OwnerMenu->EndSelection_Implementation();
			OwnerMenu->NavigateToMenu(FName("Main"), NAME_None);
		}
	}
}
