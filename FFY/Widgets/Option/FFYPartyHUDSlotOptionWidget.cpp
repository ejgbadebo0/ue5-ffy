// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYPartyHUDSlotOptionWidget.h"

#include "FFYBattleCharacterOptionWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/RichTextBlock.h"
#include "FFY/FFYBattleCharacter.h"
#include "FFY/Widgets/FFYMenuWidget.h"

void UFFYPartyHUDSlotOptionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UFFYPartyHUDSlotOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFFYPartyHUDSlotOptionWidget::OnActionSelected(AFFYAction* Action, bool bIsContext)
{
	if (bIsContext)
	{
		if (Action == nullptr)
		{
			ATBContextCostBar->SetPercent(0.f);
		}
		else
		{
			ATBContextCostBar->SetPercent(Action->ATBCost/100.0f);
		}
	}
	else
	{
		if (Action == nullptr)
		{
			ATBCostBar->SetPercent(0.0f);
			ATBVariableCost = false;
		}
		else
		{
			if (Action->ATBCost < 0)
			{
				ATBCostBar->SetPercent(Action->GetVariableATBCost(BattleCharacterReference)/100.0f);
				ATBVariableCost = true;
			} else
			{
				ATBCostBar->SetPercent(Action->ATBCost/100.0f);
			}
		
		}
	}

}

void UFFYPartyHUDSlotOptionWidget::UpdateATBProgress(float Value)
{
	ATBProgressBar->SetPercent(Value/100.0f);
	if (ATBVariableCost)
	{
		ATBCostBar->SetPercent(Value/100.0f);
	}
	OnUpdateATBProgress(Value);
}

void UFFYPartyHUDSlotOptionWidget::BattleStatsChanged(FBattleCharacterData& NewData)
{
	HPValue->SetText(FText::AsNumber(NewData.HP));
	MaxHPValue->SetText(FText::AsNumber(NewData.MaxHP));
\
	if ((NewData.HP/NewData.MaxHP) < 0.25f)
	{
		FText HPText = FText::AsNumber(NewData.HP);
		FText Status = (NewData.HP <= 0.f) ? FText::FromString("<KO>") : FText::FromString("<Weak>");
		FTextFormat FMT = FTextFormat::FromString("{SFMT}{Value}</>");
		FFormatNamedArguments Args;
		Args.Add("SFMT", Status);
		Args.Add("Value", HPText);
		HPText = FText::Format(FMT, Args);
		HPValue->SetText(HPText);
	}

	/*
	HPValue->SetDefaultColorAndOpacity(FSlateColor(FLinearColor(0.85, 0.75, 0, 1)));
	*/
	
	MPValue->SetText(FText::AsNumber(NewData.MP));
	MaxMPValue->SetText(FText::AsNumber(NewData.MaxMP));
	
}

void UFFYPartyHUDSlotOptionWidget::OnSelected()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, "Hovered");
	Super::OnSelected();
	OnPartyHUDSlotSelected.Broadcast();
}

void UFFYPartyHUDSlotOptionWidget::OnUnselected()
{
	Super::OnUnselected();
	OnPartyHUDSlotUnselected.Broadcast();
}

void UFFYPartyHUDSlotOptionWidget::PerformAction()
{
	Super::PerformAction();
}

UFFYOptionWidget* UFFYPartyHUDSlotOptionWidget::Flicker_Implementation(bool Value)
{
	if (BattleCharacterReference->SelectionWidget)
	{
		BattleCharacterReference->SelectionWidget->Flicker_Implementation(Value);
	}
	return Super::Flicker_Implementation(Value);
}

//PROXY EVENTS:
void UFFYPartyHUDSlotOptionWidget::OnSelectedProxy()
{
	OnSelected();
}

void UFFYPartyHUDSlotOptionWidget::OnUnselectedProxy()
{
	OnUnselected();
}

void UFFYPartyHUDSlotOptionWidget::PerformActionProxy()
{
	PerformAction();
}
//======================

void UFFYPartyHUDSlotOptionWidget::InitializeCharacter(AFFYBattleCharacter* CharacterReference)
{
	//GEngine->AddOnScreenDebugMessage(-1, 25.f, FColor::Green, "UFFYPartyHUDSlotOptionWidget::Initialize");
	BattleCharacterReference = CharacterReference;

	if (BattleCharacterReference)
	{
		if (BattleCharacterReference->PortraitImage)
		{
			UMaterialInstanceDynamic* MaterialInstance = Portrait->GetDynamicMaterial();
			if (MaterialInstance)
			{
				MaterialInstance->SetTextureParameterValue("InTexture", BattleCharacterReference->PortraitImage);
			}
		}
	
		CharacterName->SetText(FText::FromName(BattleCharacterReference->BattleCharacterStats.CharacterName));
		HPValue->SetText(FText::AsNumber(BattleCharacterReference->BattleCharacterStats.HP));
		MaxHPValue->SetText(FText::AsNumber(BattleCharacterReference->BattleCharacterStats.MaxHP));
		MPValue->SetText(FText::AsNumber(BattleCharacterReference->BattleCharacterStats.MP));
		MaxMPValue->SetText(FText::AsNumber(BattleCharacterReference->BattleCharacterStats.MaxMP));

		//Default ATB values
		ATBProgressBar->SetPercent(BattleCharacterReference->ATB/100.f);
		ATBCostBar->SetPercent(0.f);
		ATBContextCostBar->SetPercent(0.f);

		//Bind delegates
		BattleCharacterReference->OnCharacterStatsChanged.AddUniqueDynamic(this, &UFFYPartyHUDSlotOptionWidget::BattleStatsChanged);
		BattleCharacterReference->OnATBValueChanged.AddUniqueDynamic(this, &UFFYPartyHUDSlotOptionWidget::UpdateATBProgress);
		BattleCharacterReference->OnActionSelected.AddUniqueDynamic(this, &UFFYPartyHUDSlotOptionWidget::OnActionSelected);


		if (BattleCharacterReference->SelectionWidget)
		{
			//Link character's widget component button to this option
			BattleCharacterReference->SelectionWidget->InitializeFromWidget(this);
			BattleCharacterReference->SelectionWidget->SetOwnerMenu(OwnerMenu);
			//Bind button's delegates to synchronize events
			BattleCharacterReference->SelectionWidget->OnBattleCharacterSelected.AddUniqueDynamic(this, &UFFYPartyHUDSlotOptionWidget::OnSelectedProxy);
			BattleCharacterReference->SelectionWidget->OnBattleCharacterUnselected.AddUniqueDynamic(this, &UFFYPartyHUDSlotOptionWidget::OnUnselectedProxy);
			BattleCharacterReference->SelectionWidget->OnBattleCharacterConfirmed.AddUniqueDynamic(this, &UFFYPartyHUDSlotOptionWidget::PerformActionProxy);
		}
	}
}
