// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYMenuWidget.h"

#include "FFYMasterWidget.h"
#include "FFYOptionWidget.h"
#include "Components/WrapBox.h"
#include "FFY/FFYDataEnums.h"
#include "FFY/FFYGameInstance.h"
#include "Menu/FFYSelectPartyMemberWidget.h"

void UFFYMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

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
		}
		if (bHasCustomSelectionWidget)
		{
			if (GetSelectionWidget_Implementation())
			{
				UFFYSelectPartyMemberWidget* SelectionWidget = GetSelectionWidget_Implementation();
				SelectionWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}

		bIsInitialized = true;
	}
}

void UFFYMenuWidget::NativeDestruct()
{
	if (GetSelectionWidget())
	{
		EndSelection_Implementation();
	}
	Super::NativeDestruct();
}

void UFFYMenuWidget::Refresh()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "UFFYMenuWidget::Refresh");
}

void UFFYMenuWidget::ClearOptions()
{
	Options.Empty();
	if (WrapBox)
	{
		WrapBox->ClearChildren();
	}
}



void UFFYMenuWidget::NavigateToMenu(FName Menu, FName ContextName)
{
	if (OwnerMaster)
	{
		EndSelection_Implementation();
		OwnerMaster->NavigateToMenu(Menu, ContextName);
	}
}

void UFFYMenuWidget::CycleContextIndex(int DeltaIndex)
{
	ContextIndex += DeltaIndex;
	
	UFFYGameInstance* GameInstance = Cast<UFFYGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		int LastIndex = GameInstance->GetParty().Num() - 1;
		if (ContextIndex > LastIndex)
		{
			LoadContext_Implementation(GameInstance->GetParty()[0].PartyCharacterData.CharacterName);
			ContextIndex = 0;
		}
		if (ContextIndex < 0)
		{
			LoadContext_Implementation(GameInstance->GetParty()[LastIndex].PartyCharacterData.CharacterName);
			ContextIndex = LastIndex;
		}
		else
		{
			LoadContext_Implementation(GameInstance->GetParty()[ContextIndex].PartyCharacterData.CharacterName);
		}
	}
}

UFFYSelectPartyMemberWidget* UFFYMenuWidget::GetSelectionWidget_Implementation()
{
	return nullptr;
}


void UFFYMenuWidget::ResetOptions_Implementation()
{
	for (auto o : Options)
	{
		if (o)
		{
			o->OnUnselected();
		}
	}
}

void UFFYMenuWidget::PartyMemberSelect_Implementation(UFFYPartyMemberOptionWidget* CharacterWidget)
{
	IFFYWidgetEvents::PartyMemberSelect_Implementation(CharacterWidget);
}

void UFFYMenuWidget::SetDefaultTargetGroup_Implementation(bool bIsEnemy, bool Reset)
{
	if (GetSelectionWidget())
	{
		GetSelectionWidget()->SetDefaultTargetGroup_Implementation(bIsEnemy, Reset);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Black, "SELECTWIDGET INVALID!!!!!!!!!!!");
	}
}

void UFFYMenuWidget::EndSelection_Implementation()
{
	if (CurrentMenuMode == EMenuMode::SELECTING)
	{
		CurrentOption->Flicker_Implementation(false);
		SetMenuMode(EMenuMode::NONE);
		//minimize Selection Widget
		UFFYSelectPartyMemberWidget* SelectionWidget = GetSelectionWidget_Implementation();
		if (SelectionWidget && SelectionWidget->IsVisible())
		{
			SelectionWidget->ResetOptions_Implementation(); 
			SelectionWidget->OnPartyMemberSelected.RemoveDynamic(this, &UFFYMenuWidget::ExecuteContextAction);
			SelectionWidget->SetDefaultTargetGroup_Implementation(false, true);
			
			if (bHasCustomSelectionWidget)
			{
				SelectionWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UFFYMenuWidget::StartSelection_Implementation(UFFYOptionWidget* SelectedOption, ETargetType SelectedTargetType)
{
	SetCurrentOption(SelectedOption);
	CurrentOption->Flicker_Implementation(true);
	SetMenuMode(EMenuMode::SELECTING);
	// bring up Selection Widget
	UFFYSelectPartyMemberWidget* SelectionWidget = GetSelectionWidget_Implementation();
	if (SelectionWidget)
	{
		//FText SEText;
		//UEnum::GetDisplayValueAsText(GetVisibility(), SEText);
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, FString::Printf(TEXT("VISIBILTY: %s"), *SEText.ToString()));
		
		SelectionWidget->SetVisibility(SelectionWidget->DefaultActiveVisibility);
		SelectionWidget->OnPartyMemberSelected.AddUniqueDynamic(this, &UFFYMenuWidget::ExecuteContextAction);
		switch (SelectedTargetType)
		{
			default:
				break;
			case ETargetType::SINGLE:
				SelectionWidget->SetSelectAll(false);
				SelectionWidget->bCanSelectAll = false;
				SelectionWidget->GetOptions()[0]->OnSelected();
				break;
			case ETargetType::MULTI: //temp allow to lock selection in multi-target mode
				SelectionWidget->bCanSelectAll = true;
				SelectionWidget->SetSelectAll(true);
				SelectionWidget->bCanSelectAll = false;
				break;
			case ETargetType::BOTH:
				SelectionWidget->bCanSelectAll = true;
				SelectionWidget->GetOptions()[0]->OnSelected();
				break;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "UNABLE TO FIND SELECTION MENU");
	}
}

void UFFYMenuWidget::SelectAllInputEvent_Implementation()
{
	if (GetSelectionWidget())
	{
		GetSelectionWidget()->ToggleSelectionMode();
	}
}

void UFFYMenuWidget::SwitchTargetsInputEvent_Implementation()
{
	if (CurrentMenuMode == EMenuMode::SELECTING)
	{
		bool Result = GetSelectionWidget()->GetDefaultTargetGroup_Implementation();

		//FString Valstr = (Result) ? "true" : "false";
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Black, FString::Printf(TEXT("%s"), *Valstr));
		
		SetDefaultTargetGroup_Implementation( !Result, false);
		
	}
}

void UFFYMenuWidget::CycleInputEvent_Implementation(int DeltaIndex)
{
	if (CurrentMenuMode == EMenuMode::SELECTING)
	{
		EndSelection_Implementation();
	}
	CycleContextIndex(DeltaIndex);
}
