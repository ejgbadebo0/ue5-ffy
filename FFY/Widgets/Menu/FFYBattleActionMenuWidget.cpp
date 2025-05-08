// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYBattleActionMenuWidget.h"

#include "Components/WrapBox.h"
#include "FFY/FFYActionContainer.h"
#include "FFY/FFYBattleCharacter.h"
#include "FFY/Widgets/Option/FFYBattleActionOptionWidget.h"

void UFFYBattleActionMenuWidget::LoadBattleContext_Implementation(AFFYBattleCharacter* Character)
{
	ClearOptions();
	
	if (Character)
	{
		switch (ActionMenuType)
		{
		default:
			break;
		case EActionMenuType::NONE:
			break;
		case EActionMenuType::MAGIC:
			
			if (Character->ActionContainer)
			{
				for (auto a : Character->ActionContainer->Magic)
				{
					if (a)
					{
						auto ToAdd = CreateWidget<UFFYBattleActionOptionWidget>(this, ActionOptionClass);
						if (ToAdd)
						{
							ToAdd->InitializeOption(a,0);
							
							Options.Emplace(ToAdd);
							ToAdd->SetOwnerMenu(this);
							if (WrapBox)
							{
								WrapBox->AddChildToWrapBox(ToAdd);
							}
						}
					}
				}
			}
			break;
		}
	}
}

UFFYSelectPartyMemberWidget* UFFYBattleActionMenuWidget::GetSelectionWidget_Implementation()
{
	return OwnerMaster->GetMasterSelectionWidget();
}
