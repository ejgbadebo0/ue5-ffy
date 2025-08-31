// Source code implementation by Ephraim Gbadebo.


#include "FFYSlotEquipOptionWidget.h"

#include "Components/Image.h"
#include "FFY/FFYDataEnums.h"
#include "FFY/Widgets/FFYMenuWidget.h"

void UFFYSlotEquipOptionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UFFYSlotEquipOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFFYSlotEquipOptionWidget::SetEquipmentName(FName EquipmentName)
{
	if (EquipmentName != NAME_None)
	{
		DisplayText->SetText(FText::FromName(EquipmentName));
	}
	else
	{
		DisplayText->SetText(FText::FromString(""));
	}
}

FText UFFYSlotEquipOptionWidget::GetDescription()
{
	return Super::GetDescription();
}

void UFFYSlotEquipOptionWidget::OnSelected()
{
	if (SelectedImage && !bIsFlickering && OwnerMenu->GetCurrentMenuMode() == EMenuMode::NONE)
	{
		SelectedImage->SetVisibility(ESlateVisibility::Visible);
		if (OwnerMenu)
		{
			OwnerMenu->SetCurrentOption(this); 
		}
	}
}

void UFFYSlotEquipOptionWidget::OnUnselected()
{
	if (SelectedImage && !bIsFlickering && OwnerMenu->GetCurrentMenuMode() == EMenuMode::NONE)
	{
		SelectedImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UFFYSlotEquipOptionWidget::PerformAction()
{
	if ( bIsDisabled || OwnerMenu->GetCurrentMenuMode() != EMenuMode::NONE )
	{
		return;
	}
	OwnerMenu->LoadEquipmentItems_Implementation(this->EquipmentClass, this->EquipmentSlot);
	OwnerMenu->StartSelection_Implementation(this, ETargetType::SINGLE);
	
}

UFFYOptionWidget* UFFYSlotEquipOptionWidget::Flicker_Implementation(bool Value)
{
	UFFYOptionWidget* Return = Super::Flicker_Implementation(Value);
	if (!Value)
	{
		SelectedImage->SetVisibility(ESlateVisibility::Hidden);
	}
	return Return;
}
