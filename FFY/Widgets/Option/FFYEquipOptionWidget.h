// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/FFYWidgetEvents.h"
#include "FFY/Widgets/FFYOptionWidget.h"
#include "FFYEquipOptionWidget.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API UFFYEquipOptionWidget : public UFFYOptionWidget, public IFFYWidgetEvents
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;

public:
	virtual FText GetDescription() override
	{
		return FText::FromString("Customize party members' currently equipped items.");
	}

	virtual void OnSelected() override;
	virtual void OnUnselected() override;
	virtual void PerformAction() override;

	virtual void ContextAction_Implementation(UFFYPartyMemberOptionWidget* CharacterWidget, bool SelectAll) override;
};
