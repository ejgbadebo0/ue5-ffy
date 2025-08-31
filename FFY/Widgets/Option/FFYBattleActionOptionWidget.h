// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Widgets/Option/FFYActionOptionWidget.h"
#include "FFYBattleActionOptionWidget.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API UFFYBattleActionOptionWidget : public UFFYActionOptionWidget
{
	GENERATED_BODY()

public:
	virtual void PerformAction() override;
	virtual void OnSelected() override;
	virtual void OnUnselected() override;
	
	virtual void BattleContextAction_Implementation(AFFYBattleCharacter* Character, bool SelectAll) override;
	virtual void ContextAction_Implementation(UFFYPartyMemberOptionWidget* CharacterWidget, bool SelectAll) override;
};
