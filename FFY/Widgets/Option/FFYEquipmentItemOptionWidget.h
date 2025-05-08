// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Widgets/Option/FFYInventoryItemOptionWidget.h"
#include "FFYEquipmentItemOptionWidget.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API UFFYEquipmentItemOptionWidget : public UFFYInventoryItemOptionWidget
{
	GENERATED_BODY()

public:
	virtual void OnSelected() override;
	virtual FText GetDescription() override;
	virtual void OnUnselected() override;
	virtual void PerformAction() override;
	
	//virtual void ContextAction_Implementation(UFFYPartyMemberOptionWidget* CharacterWidget) override;
};
