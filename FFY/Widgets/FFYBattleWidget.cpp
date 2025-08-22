// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYBattleWidget.h"

#include "Components/WrapBox.h"
#include "FFY/FFYActionContainer.h"
#include "Menu/FFYActionQueueMenuWidget.h"
#include "Menu/FFYContextCommandMenuWidget.h"
#include "Menu/FFYSelectBattleCharacterWidget.h"
#include "Option/FFYPartyHUDSlotOptionWidget.h"

void UFFYBattleWidget::AddHUDSlot(AFFYBattleCharacter* Character)
{
	UFFYSelectBattleCharacterWidget* Widget = Cast<UFFYSelectBattleCharacterWidget>(SelectionWidget);
	if (Character && Widget)
	{
		Widget->AddHUDSlot(Character);
		if (!Widget->OnOptionsUpdated.IsBound())
		{
			Widget->OnOptionsUpdated.AddUniqueDynamic(this, &UFFYBattleWidget::RefreshBattleContext); 
		}
	}
	
}

void UFFYBattleWidget::InitializeContextMenu(TArray<AFFYBattleCharacter*> Party)
{
	if (ContextCommandMenu)
	{
		ContextCommandMenu->InitializeParty(Party);
	}

	if (ActionQueueMenu)
	{
		ActionQueueMenu->InitializeParty(Party);
	}
}

void UFFYBattleWidget::EndSelection_Implementation()
{
	Super::EndSelection_Implementation();
}

void UFFYBattleWidget::CycleInputEvent_Implementation(int DeltaIndex)
{
	Super::CycleInputEvent_Implementation(DeltaIndex);
}

void UFFYBattleWidget::DefendInputEvent_Implementation()
{
	Super::DefendInputEvent_Implementation();

	if (BattleContext)
	{
		for (auto a : BattleContext->ActionContainer->MainCommands)
		{
			if (a && a->Label == FName("Defend"))
			{
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "DEFEND");
				break;
			}
		}
	}
}

void UFFYBattleWidget::ContextCommandInputEvent_Implementation(int DeltaIndex)
{
	if (ContextCommandMenu)
	{
		ContextCommandMenu->ContextCommandInputEvent_Implementation(DeltaIndex);
	}
}

void UFFYBattleWidget::LoadBattleContext_Implementation(AFFYBattleCharacter* Character)
{
	Super::LoadBattleContext_Implementation(Character);
	
	//Cancel any ongoing context widget events and reset for new character
	EndSelection_Implementation();
	IFFYWidgetEvents* ActiveWidget = Cast<IFFYWidgetEvents>(WidgetSwitcher->GetActiveWidget());
	if (ActiveWidget)
	{
		ActiveWidget->ResetOptions_Implementation();
	}
	NavigateToMenu(FName("Main"), NAME_None);
	
	//Set character menus
	if (Character)
	{
		SetBattleContext_Implementation(Character);
	
		if (WidgetSwitcher)
		{
			for (auto i : WidgetSwitcher->GetAllChildren())
			{
				IFFYWidgetEvents* Widget = Cast<IFFYWidgetEvents>(i);
				if (Widget)
				{
					Widget->LoadBattleContext_Implementation(BattleContext);
				}
			}
		}
	}

}

void UFFYBattleWidget::SelectAllInputEvent_Implementation()
{
	Super::SelectAllInputEvent_Implementation();
}

UFFYSelectPartyMemberWidget* UFFYBattleWidget::GetMasterSelectionWidget_Implementation()
{
	return SelectionWidget;
}

void UFFYBattleWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UFFYBattleWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
