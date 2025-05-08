// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYMenuScrollWidget.h"
//ext includes
#include "FFYOptionWidget.h"
#include "Components/ScrollBox.h"
#include "Components/WrapBox.h"

void UFFYMenuScrollWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UFFYMenuScrollWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UFFYMenuScrollWidget::ClearOptions()
{
	Super::ClearOptions();
}

void UFFYMenuScrollWidget::Refresh()
{
	ClearOptions();
}
