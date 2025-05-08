// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Widgets/FFYOptionWidget.h"
#include "FFYItemsOptionWidget.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API UFFYItemsOptionWidget : public UFFYOptionWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	virtual void OnSelected() override;
	virtual void OnUnselected() override;
	virtual void PerformAction() override;
	
protected:
	virtual FText GetDescription() override
	{
		return FText::FromString("View and use items from inventory.");
	}
	
};
