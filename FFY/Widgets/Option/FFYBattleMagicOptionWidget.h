// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Widgets/Option/FFYMagicOptionWidget.h"
#include "FFYBattleMagicOptionWidget.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API UFFYBattleMagicOptionWidget : public UFFYMagicOptionWidget
{
	GENERATED_BODY()

public:
	virtual void PerformAction() override;
};
