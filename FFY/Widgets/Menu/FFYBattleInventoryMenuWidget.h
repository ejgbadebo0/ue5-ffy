// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Widgets/Menu/FFYInventoryMenuWidget.h"
#include "FFYBattleInventoryMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API UFFYBattleInventoryMenuWidget : public UFFYInventoryMenuWidget
{
	GENERATED_BODY()

public:
	virtual UFFYSelectPartyMemberWidget* GetSelectionWidget_Implementation() override
	{
		return OwnerMaster->GetMasterSelectionWidget();
	}

	virtual void SwitchTargetsInputEvent_Implementation() override
	{
		Super::SwitchTargetsInputEvent_Implementation();
	}
	
	virtual FText GetInputKeyText_Implementation() override
	{
		return FText::FromString("<img id=\"LeftMouseButton\"></> = Confirm\n \n <img id=\"RightMouseButton\"></> = Cancel\n \n <img id=\"C\"></> = Select All\n \n <img id=\"X\"></> = Switch Target Group\n \n");
	}

	virtual void SetDefaultTargetGroup_Implementation(bool bIsEnemy, bool Reset) override
	{
		Super::SetDefaultTargetGroup_Implementation(bIsEnemy, Reset);
	}
};
