// Source code implementation by Ephraim Gbadebo.


#include "FFYBattleMainMenuWidget.h"

#include "FFYSelectPartyMemberWidget.h"
#include "Components/WrapBox.h"
#include "FFY/FFYBattleCharacter.h"
#include "FFY/Actions/FFYAttackAction.h"
#include "FFY/Widgets/Option/FFYBattleActionOptionWidget.h"
#include "FFY/Widgets/Option/FFYBattleMagicOptionWidget.h"
#include "FFY/Widgets/Option/FFYItemsOptionWidget.h"

void UFFYBattleMainMenuWidget::LoadBattleContext_Implementation(AFFYBattleCharacter* Character)
{
	Super::LoadBattleContext_Implementation(Character);

	ClearOptions();
	
	//Look for main battle menu options and arrange in set order
	for (auto a : Character->Actions)
	{
		if (a->Label == FName("Attack"))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Emerald, "FOUND ATTACK ACTION");
			auto ToAdd = CreateWidget<UFFYBattleActionOptionWidget>(this, ActionOptionWidgetClass);
			if (ToAdd)
			{
				ToAdd->InitializeOption(a, 0);
				ToAdd->SetOwnerMenu(this);
				Options.EmplaceAt(0, ToAdd);
				if (WrapBox)
				{
					WrapBox->AddChildToWrapBox(ToAdd);
				}
			}
			
		}
		if (a->Label == FName("Magic"))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Emerald, "FOUND MAGIC ACTION");
			auto ToAdd = CreateWidget<UFFYBattleMagicOptionWidget>(this, MagicOptionWidgetClass);
			if (ToAdd)
			{
				ToAdd->SetOwnerMenu(this);
				Options.EmplaceAt(1, ToAdd);
				if (WrapBox)
				{
					WrapBox->AddChildToWrapBox(ToAdd);
				}
			}
		}
		if (a->Label == FName("Item"))
		{
			auto ToAdd = CreateWidget<UFFYItemsOptionWidget>(this, ItemsOptionWidgetClass);
			if (ToAdd)
			{
				ToAdd->SetOwnerMenu(this);
				Options.Emplace(ToAdd);
				if (WrapBox)
				{
					WrapBox->AddChildToWrapBox(ToAdd);
				}
			}
		}
	}
}

void UFFYBattleMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFFYBattleMainMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

UFFYSelectPartyMemberWidget* UFFYBattleMainMenuWidget::GetSelectionWidget_Implementation()
{
	return OwnerMaster->GetMasterSelectionWidget();
}

void UFFYBattleMainMenuWidget::SetDefaultTargetGroup_Implementation(bool bIsEnemy, bool Reset)
{
	Super::SetDefaultTargetGroup_Implementation(bIsEnemy, Reset);
}
