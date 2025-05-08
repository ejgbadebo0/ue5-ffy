// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Widgets/FFYMenuWidget.h"
#include "FFYMainPauseMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API UFFYMainPauseMenuWidget : public UFFYMenuWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

public:

	virtual UFFYSelectPartyMemberWidget* GetSelectionWidget_Implementation() override;
	
	virtual void EndSelection_Implementation() override;
	
	virtual void StartSelection_Implementation(UFFYOptionWidget* SelectedOption, ETargetType SelectedTargetType) override;
};
