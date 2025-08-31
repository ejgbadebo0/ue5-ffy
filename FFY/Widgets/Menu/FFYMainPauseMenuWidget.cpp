// Source code implementation by Ephraim Gbadebo.


#include "FFYMainPauseMenuWidget.h"

#include "FFYSelectPartyMemberWidget.h"
#include "Components/WrapBox.h"
#include "FFY/FFYDataEnums.h"

void UFFYMainPauseMenuWidget::NativeConstruct()
{
	
	if (!bIsInitialized)
	{
		for (auto i : OptionClasses)
		{
			if (i)
			{
				auto ToAdd = CreateWidget<UFFYOptionWidget>(this, i);
				if (ToAdd)
				{
					ToAdd->SetOwnerMenu(this);
					Options.Add(ToAdd);
					if (WrapBox)
					{
						WrapBox->AddChildToWrapBox(ToAdd);
					}
				}
			}
		}/*
		UFFYSelectPartyMemberWidget* SelectionWidget = GetSelectionWidget_Implementation();
		if (SelectionWidget)
		{
			SelectionWidget->SetVisibility(ESlateVisibility::Visible);
		}*/
		bIsInitialized = true;
	}

}

void UFFYMainPauseMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

UFFYSelectPartyMemberWidget* UFFYMainPauseMenuWidget::GetSelectionWidget_Implementation()
{
	return OwnerMaster->GetMasterSelectionWidget();
}

void UFFYMainPauseMenuWidget::EndSelection_Implementation()
{
	if (CurrentMenuMode == EMenuMode::SELECTING)
	{
		CurrentOption->Flicker_Implementation(false);
		SetMenuMode(EMenuMode::NONE);
		//minimize Selection Widget
		UFFYSelectPartyMemberWidget* SelectionWidget = GetSelectionWidget_Implementation();
		if (SelectionWidget && SelectionWidget->IsVisible())
		{
			SelectionWidget->SetGuidedSelect(false);
			SelectionWidget->OnPartyMemberSelected.RemoveDynamic(this, &UFFYMenuWidget::ExecuteContextAction);
			SelectionWidget->GetOptions()[0]->OnUnselected();
		}
	}
}

void UFFYMainPauseMenuWidget::StartSelection_Implementation(UFFYOptionWidget* SelectedOption, ETargetType SelectedTargetType)
{
	SetCurrentOption(SelectedOption);
	CurrentOption->Flicker_Implementation(true);
	SetMenuMode(EMenuMode::SELECTING);
	// bring up Selection Widget
	UFFYSelectPartyMemberWidget* SelectionWidget = GetSelectionWidget_Implementation();
	if (SelectionWidget)
	{
		SelectionWidget->OnPartyMemberSelected.AddUniqueDynamic(this, &UFFYMenuWidget::ExecuteContextAction);
		SelectionWidget->GetOptions()[0]->OnSelected();
		SelectionWidget->SetGuidedSelect(true);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "UNABLE TO FIND SELECTION MENU");
	}
}
