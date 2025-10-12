// Source code implementation by Ephraim Gbadebo.


#include "FFYConfirmMenuWidget.h"

void UFFYConfirmMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Collapsed);
}

void UFFYConfirmMenuWidget::SetConfirmCallback(FName CallbackFunctionName)
{
	ConfirmCallback = CallbackFunctionName;
	
}

void UFFYConfirmMenuWidget::ExecuteConfirmCallback(bool bIsConfirmed)
{
	OnConfirmOptionSelected.Broadcast(bIsConfirmed);
}
