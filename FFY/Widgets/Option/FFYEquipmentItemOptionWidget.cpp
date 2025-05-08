// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYEquipmentItemOptionWidget.h"

#include "Components/Image.h"
#include "FFY/Widgets/FFYMenuWidget.h"

void UFFYEquipmentItemOptionWidget::OnSelected()
{
	//Super::OnSelected();
	SelectedImage->SetVisibility(ESlateVisibility::Visible);
	OwnerMenu->EquipmentItemHovered_Implementation(this);
}

FText UFFYEquipmentItemOptionWidget::GetDescription()
{
	return Super::GetDescription();
}

void UFFYEquipmentItemOptionWidget::OnUnselected()
{
	//Super::OnUnselected();
	SelectedImage->SetVisibility(ESlateVisibility::Collapsed);
	OwnerMenu->EquipmentItemUnhovered_Implementation(this);
}

void UFFYEquipmentItemOptionWidget::PerformAction()
{
	Super::PerformAction();
	OwnerMenu->EquipmentItemSelected_Implementation(this);
	
}
