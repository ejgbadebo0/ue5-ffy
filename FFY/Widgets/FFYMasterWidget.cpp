// Source code implementation by Ephraim Gbadebo.


#include "FFYMasterWidget.h"

#include "FFYMenuWidget.h"
#include "Components/RichTextBlock.h"
#include "FFY/FFYDataEnums.h"
#include "Menu/FFYSelectPartyMemberWidget.h"

void UFFYMasterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (WidgetSwitcher)
	{
		for (auto c : WidgetSwitcher->GetAllChildren())
		{
			if (c)
			{
				UFFYMenuWidget* Menu = dynamic_cast<UFFYMenuWidget*>(c);
				if (Menu)
				{
					Menu->SetOwnerMaster(this);
				}
			}
		}
	}
	CurrentMenu = FName("Main");
	if (InputKeyLegend)
	{
		IFFYWidgetEvents* Widget = Cast<IFFYWidgetEvents>(WidgetSwitcher->GetActiveWidget());
		if (Widget)
		{
			InputKeyLegend->SetText(Widget->GetInputKeyText_Implementation());
			InputKeyLegend->SetVisibility(( InputKeyLegend->GetText().IsEmpty()) ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		}
	}
	
}

void UFFYMasterWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (WidgetSwitcher)
	{
		NavigateToMenu(FName("Main"), NAME_None);
	}
	if (bShouldKeepTime)
	{
		
	}
}

void UFFYMasterWidget::NavigateToMenu(FName Menu, FName ContextName)
{
	if (Menus.Contains(Menu))
	{
		int* NextMenuIndex = Menus.Find(Menu);
		IFFYWidgetEvents* Widget = Cast<IFFYWidgetEvents>(WidgetSwitcher->GetWidgetAtIndex(*NextMenuIndex));
		if (Widget)
		{
			InputKeyLegend->SetText(Widget->GetInputKeyText_Implementation());
			InputKeyLegend->SetVisibility(( InputKeyLegend->GetText().IsEmpty()) ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
			
			if (ContextName != NAME_None)
			{
				Widget->LoadContext_Implementation(ContextName);
			}
		}
		WidgetSwitcher->SetActiveWidgetIndex(*NextMenuIndex);
		CurrentMenu = Menu;

		OnMenuChanged.Broadcast();
	}
}

void UFFYMasterWidget::PreviousMenu()
{

	if (CurrentMenu.IsValid())
	{
		FName* Previous = PreviousMenus.Find(CurrentMenu);
		NavigateToMenu(*Previous, NAME_None);
	}
	
}

UFFYSelectPartyMemberWidget* UFFYMasterWidget::GetMasterSelectionWidget_Implementation()
{
	return SelectionWidget;
}

void UFFYMasterWidget::EndSelection_Implementation()
{
	SetMenuMode(EMenuMode::NONE);
	IFFYWidgetEvents* Widget = Cast<IFFYWidgetEvents>(WidgetSwitcher->GetActiveWidget());
	if (Widget)
	{
		Widget->EndSelection_Implementation();
	}
}
