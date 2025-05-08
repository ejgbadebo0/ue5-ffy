// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FFY/FFYWidgetEvents.h"
#include "FFY/Widgets/FFYOptionWidget.h"
#include "FFYStatusOptionWidget.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API UFFYStatusOptionWidget : public UFFYOptionWidget, public IFFYWidgetEvents
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;

public:
	virtual FText GetDescription() override
	{
		return FText::FromString("View current stats of party members.");
	}

	virtual void OnSelected() override;
	virtual void OnUnselected() override;
	virtual void PerformAction() override;

	virtual void ContextAction_Implementation(UFFYPartyMemberOptionWidget* CharacterWidget, bool SelectAll) override;
};
