// Source code implementation by Ephraim Gbadebo.


#include "FFYBattleMagicOptionWidget.h"

#include "FFY/Widgets/FFYMenuWidget.h"

void UFFYBattleMagicOptionWidget::PerformAction()
{
	if ( bIsDisabled || OwnerMenu->GetCurrentMenuMode() != EMenuMode::NONE )
	{
		return;
	}
	OwnerMenu->NavigateToMenu(FName("Magic"), NAME_None);
}
