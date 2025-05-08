// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYBattleCharacterOptionWidget.h"

#include "FFYPartyHUDSlotOptionWidget.h"
#include "Components/Image.h"
#include "FFY/Widgets/FFYMenuWidget.h"
#include "FFY/Widgets/Menu/FFYSelectBattleCharacterWidget.h"

void UFFYBattleCharacterOptionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UFFYBattleCharacterOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFFYBattleCharacterOptionWidget::ShouldDisable(bool bIsEnemy)
{
	//FString Valstr = (bIsEnemy == Enemy) ? "true" : "false";
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("%s"), *Valstr));
	SetIsDisabled((bIsEnemy != Enemy));
	
}

void UFFYBattleCharacterOptionWidget::OnReset()
{
	SetIsDisabled(false);
}

void UFFYBattleCharacterOptionWidget::InitializeFromWidget(UFFYPartyHUDSlotOptionWidget* InWidget)
{
	InWidget->OnPartyHUDSlotSelected.AddUniqueDynamic(this, &UFFYBattleCharacterOptionWidget::OnSelectedProxy);
	InWidget->OnPartyHUDSlotUnselected.AddUniqueDynamic(this, &UFFYBattleCharacterOptionWidget::OnUnselectedProxy);

	auto InMenu = Cast<UFFYSelectBattleCharacterWidget>(InWidget->GetOwnerMenu());
	if (InMenu)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, "BCMenu Linked");
		InMenu->OnOptionGroupDisabled.AddUniqueDynamic(this, &UFFYBattleCharacterOptionWidget::ShouldDisable);
		InMenu->OnResetAll.AddUniqueDynamic(this, &UFFYBattleCharacterOptionWidget::OnReset);
	}
}

//Manual override to avoid setting menu's CurrentOption
void UFFYBattleCharacterOptionWidget::OnSelected()
{
	
	bIsHovered = true;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, "BC Option Hovered");
	if (SelectedImage && !bIsFlickering && !bIsDisabled)
	{
		SelectedImage->SetVisibility(ESlateVisibility::Visible);
		OnBattleCharacterSelected.Broadcast();
	}
}

void UFFYBattleCharacterOptionWidget::OnUnselected()
{
	bIsHovered = false;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, "BC Unhovered");
	if (SelectedImage && !bIsFlickering && !bIsDisabled)
	{
		SelectedImage->SetVisibility(DefaultUnselectedVisibility);
		OnBattleCharacterUnselected.Broadcast();
	}
	
}

void UFFYBattleCharacterOptionWidget::PerformAction()
{
	if ( bIsDisabled )
	{
		return;
	}
	OnBattleCharacterConfirmed.Broadcast();
}


//Replicated button events from corresponding main battle UI button
//=======================
void UFFYBattleCharacterOptionWidget::OnSelectedProxy()
{
	bIsHovered = true;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, "Hovered");
	if (SelectedImage && !bIsFlickering)
	{
		SelectedImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void UFFYBattleCharacterOptionWidget::OnUnselectedProxy()
{
	bIsHovered = false;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, "Unhovered");
	if (SelectedImage && !bIsFlickering)
	{
		SelectedImage->SetVisibility(DefaultUnselectedVisibility);
	}
}

//=========================