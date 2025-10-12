// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFYConfirmMenuWidget.h"
#include "FFY/Widgets/FFYOptionWidget.h"
#include "FFYConfirmYesOptionWidget.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API UFFYConfirmYesOptionWidget : public UFFYOptionWidget
{
	GENERATED_BODY()

public:
	virtual void PerformAction() override
	{
		Super::PerformAction();

		if (OwnerMenu)
		{
			UFFYConfirmMenuWidget* ConfirmMenu = Cast<UFFYConfirmMenuWidget>(OwnerMenu);
			if (ConfirmMenu)
			{
				ConfirmMenu->ExecuteConfirmCallback(true);
			}
		}
	}
};
