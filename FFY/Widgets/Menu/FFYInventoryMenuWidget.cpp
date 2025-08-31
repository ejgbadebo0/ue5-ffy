// Source code implementation by Ephraim Gbadebo.


#include "FFYInventoryMenuWidget.h"

#include "Components/WrapBox.h"
#include "FFY/Widgets/Option/FFYInventoryItemOptionWidget.h"
#include "FFY/FFYGameInstance.h"

void UFFYInventoryMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Refresh();

	//Bing delegate to refresh the widget everytime widget is updated
	
	UFFYGameInstance* GameInstance = Cast<UFFYGameInstance>(GetGameInstance());
	if (GameInstance )
	{
		GameInstance->OnInventoryUpdated.AddUniqueDynamic(this, &UFFYInventoryMenuWidget::InactiveRefresh);
	}
}

void UFFYInventoryMenuWidget::NativeDestruct()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UFFYInventoryMenuWidget::NativeDestruct");
	Super::NativeDestruct();
}

void UFFYInventoryMenuWidget::ClearOptions()
{
	Super::ClearOptions();
}

void UFFYInventoryMenuWidget::Refresh()
{
	Super::Refresh();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UFFYInventoryMenuWidget::Refresh");
	UFFYGameInstance* GameInstance = Cast<UFFYGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		for (auto i : GameInstance->Inventory)
		{
			if (i.ID.IsValid())
			{
				auto ToAdd = CreateWidget<UFFYInventoryItemOptionWidget>(this, ItemButtonClass);
				if (ToAdd)
				{
					ToAdd->InitializeOption(i);
					ToAdd->SetOwnerMenu(this);
					Options.Add(ToAdd);
					if (WrapBox)
					{
						WrapBox->AddChildToWrapBox(ToAdd);
					}
				}
			}
		}
	}
	
}

void UFFYInventoryMenuWidget::InactiveRefresh()
{
	if (OwnerMaster->GetWidgetSwitcher()->GetActiveWidget() != this)
	{
		Refresh();
	}
}

UFFYSelectPartyMemberWidget* UFFYInventoryMenuWidget::GetSelectionWidget_Implementation()
{
	return SelectionWidget;
}
