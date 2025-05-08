// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FFY/FFYWidgetEvents.h"
#include "FFY/Widgets/FFYOptionWidget.h"
#include "FFYMagicOptionWidget.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API UFFYMagicOptionWidget : public UFFYOptionWidget, public IFFYWidgetEvents
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual FText GetDescription() override
	{
		return FText::FromString("View and use party members' magic abilities.");
	}
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	virtual void PerformAction() override;
	virtual void OnSelected() override;
	virtual void OnUnselected() override;
	virtual void ContextAction_Implementation(UFFYPartyMemberOptionWidget* CharacterWidget, bool SelectAll) override;
};
