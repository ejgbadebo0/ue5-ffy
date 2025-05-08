// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYSelectBattleCharacterWidget.h"

#include "Components/Border.h"
#include "Components/WrapBox.h"
#include "FFY/FFYBattleCharacter.h"
#include "FFY/Widgets/Option/FFYBattleCharacterOptionWidget.h"
#include "FFY/Widgets/Option/FFYEnemySlotOptionWidget.h"
#include "FFY/Widgets/Option/FFYPartyHUDSlotOptionWidget.h"

void UFFYSelectBattleCharacterWidget::AddHUDSlot(AFFYBattleCharacter* Character)
{
	if (Character == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 35, FColor::Yellow, "AddHUDSlot: null");
		return;
	}
	else
	{
		if ( Character->ActorHasTag(FName("Enemy")) )
		{
			//GEngine->AddOnScreenDebugMessage(-1, 35, FColor::Yellow, "AddHUDSlot: enemy");
			auto ToAdd = CreateWidget<UFFYEnemySlotOptionWidget>(this, EnemySlotWidgetClass);
			if (ToAdd)
			{
				//set owner before initializing for delegate bindings
				ToAdd->SetOwnerMenu(this);
				ToAdd->InitializeCharacter(Character);
				
				EnemyOptions.Emplace(ToAdd);
				
				if (EnemyList)
				{
					EnemyList->AddChildToWrapBox(ToAdd);
				}
			}
			return;
		}
		else
		{

			auto ToAdd = CreateWidget<UFFYPartyHUDSlotOptionWidget>(this, PartyButtonClass);
			if (ToAdd)
			{
				//set owner before initializing for delegate bindings
				ToAdd->SetOwnerMenu(this);
				ToAdd->InitializeCharacter(Character);
				
				Options.Emplace(ToAdd);
				PartyOptions.Emplace(ToAdd);
				if (WrapBox)
				{
					WrapBox->AddChildToWrapBox(ToAdd);
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 35, FColor::Yellow, "AddHUDSlot: no HUDSLOT/WrapBox");
			}
		}
	}
}

void UFFYSelectBattleCharacterWidget::SetDefaultTargetGroup_Implementation(bool bIsEnemy, bool Reset)
{
	//FString Valstr = (bIsEnemy) ? "true" : "false";
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("%s"), *Valstr));


	//set for input events
	bIsTargetingEnemies = bIsEnemy;

	//don't leave option of unselected group with active marker
	if (Options[0])
	{
		Options[0]->OnUnselected();
	}

	//determine if we are currently in SelectAll mode
	bool bWasSelectAll = bSelectAll;
	
	if (bWasSelectAll)
	{
		SetSelectAll(false);
	}
	
	if (bIsEnemy)
	{
		Options = EnemyOptions;
		PartyHUD->SetVisibility(ESlateVisibility::HitTestInvisible);
		EnemyHUD->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		Options = PartyOptions;
		PartyHUD->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		EnemyHUD->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	//if SelectAll mode was active before switching targets reset on new group
	if (bWasSelectAll)
	{
		SetSelectAll(true);
	}

	if (Reset)
	{
		OnResetAll.Broadcast();
	}
	else
	{
		//Select the first option in new group
		if (Options[0])
		{
			Options[0]->OnSelected();
		}
		OnOptionGroupDisabled.Broadcast(bIsEnemy);
	}
}

TArray<AFFYBattleCharacter*> UFFYSelectBattleCharacterWidget::GetTargetGroupActors_Implementation(bool bIsEnemies)
{
	
	TArray<AFFYBattleCharacter*> Targets;
		
	if (bIsEnemies)
	{
		for (auto e : EnemyOptions)
		{
			UFFYPartyHUDSlotOptionWidget* Widget = Cast<UFFYPartyHUDSlotOptionWidget>(e);
			if (Widget)
			{
				Targets.Emplace(Widget->GetBattleCharacterReference());
			}
		}
	}
	else
	{
		for (auto p : PartyOptions)
		{
			UFFYPartyHUDSlotOptionWidget* Widget = Cast<UFFYPartyHUDSlotOptionWidget>(p);
			if (Widget)
			{
				Targets.Emplace(Widget->GetBattleCharacterReference());
			}
		}
	}

	return Targets;
}

void UFFYSelectBattleCharacterWidget::ResetOptions_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Select BC Widget RESET OPTIONS");

	if (bSelectAll)
	{
		SetSelectAll(false);
	}
	bCanSelectAll = false;
	Super::ResetOptions_Implementation();
}

void UFFYSelectBattleCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFFYSelectBattleCharacterWidget::Refresh()
{
	//Super::Refresh();
}
