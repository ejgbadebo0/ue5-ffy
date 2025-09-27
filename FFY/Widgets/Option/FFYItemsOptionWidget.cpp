// Source code implementation by Ephraim Gbadebo.


#include "FFYItemsOptionWidget.h"

#include "FFY/FFYDataEnums.h"
#include "FFY/Widgets/FFYMenuWidget.h"

void UFFYItemsOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFFYItemsOptionWidget::OnSelected()
{
	Super::OnSelected();
}

void UFFYItemsOptionWidget::OnUnselected()
{
	Super::OnUnselected();
}

void UFFYItemsOptionWidget::PerformAction()
{
	Super::PerformAction();
	
	if ( bIsDisabled || OwnerMenu->GetCurrentMenuMode() != EMenuMode::NONE )
	{
		return;
	}
    //navigate to "Items" menu

	OwnerMenu->NavigateToMenu(FName("Items"), NAME_None);

}
