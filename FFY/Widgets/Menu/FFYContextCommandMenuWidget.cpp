// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYContextCommandMenuWidget.h"

void UFFYContextCommandMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (RightInputCCWidget && LeftInputCCWidget)
	{
		
		RightInputCCWidget->SetOwnerMenu(this);
		LeftInputCCWidget->SetOwnerMenu(this);

		RightInputCCWidget->SetIsDisabled(true);
		LeftInputCCWidget->SetIsDisabled(true);
		
	}
}

void UFFYContextCommandMenuWidget::LoadContextCommand(AFFYBattleCharacter* Character)
{
	if (Character)
	{
		// prioritize the right slot, if occupied use the left
		if (RightInputCCWidget->CharacterReference == nullptr || RightInputCCWidget->CharacterReference == Character)
		{
			RightInputCCWidget->Reload(Character);
		}
		else
		{
			LeftInputCCWidget->Reload(Character);
		}
	}
}

void UFFYContextCommandMenuWidget::UnloadContextCommand(AFFYBattleCharacter* Character)
{
	if (Character)
	{
		if (RightInputCCWidget->CharacterReference == Character)
		{
			RightInputCCWidget->Unload();
		}
		else
		{
			LeftInputCCWidget->Unload();
		}
	}
}
