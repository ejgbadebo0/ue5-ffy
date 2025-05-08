// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYStatusOptionWidget.h"

#include "FFYPartyMemberOptionWidget.h"
#include "FFY/Widgets/FFYMenuWidget.h"

void UFFYStatusOptionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UFFYStatusOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFFYStatusOptionWidget::OnSelected()
{
	Super::OnSelected();
}

void UFFYStatusOptionWidget::OnUnselected()
{
	Super::OnUnselected();
}

void UFFYStatusOptionWidget::PerformAction()
{
	if ( bIsDisabled || OwnerMenu->GetCurrentMenuMode() != EMenuMode::NONE )
	{
		return;
	}
	OwnerMenu->StartSelection_Implementation(this, ETargetType::SINGLE);
	OwnerMenu->SetDescription(FText::FromString("Select a party member."));
}

void UFFYStatusOptionWidget::ContextAction_Implementation(UFFYPartyMemberOptionWidget* CharacterWidget, bool SelectAll)
{
	OwnerMenu->NavigateToMenu(FName("Status"), CharacterWidget->GetCharacterName());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("STATUS BUTTON PRESSED"));
}
