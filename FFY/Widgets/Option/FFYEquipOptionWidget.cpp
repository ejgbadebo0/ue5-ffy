// Source code implementation by Ephraim Gbadebo.


#include "FFYEquipOptionWidget.h"

#include "FFYPartyMemberOptionWidget.h"
#include "FFY/FFYDataEnums.h"
#include "FFY/Widgets/FFYMenuWidget.h"

void UFFYEquipOptionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UFFYEquipOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFFYEquipOptionWidget::OnSelected()
{
	Super::OnSelected();
}

void UFFYEquipOptionWidget::OnUnselected()
{
	Super::OnUnselected();
}

void UFFYEquipOptionWidget::PerformAction()
{
	if ( bIsDisabled || OwnerMenu->GetCurrentMenuMode() != EMenuMode::NONE )
	{
		return;
	}
	OwnerMenu->StartSelection_Implementation(this, ETargetType::SINGLE);
	OwnerMenu->SetDescription(FText::FromString("Select a party member."));
}

void UFFYEquipOptionWidget::ContextAction_Implementation(UFFYPartyMemberOptionWidget* CharacterWidget, bool SelectAll)
{
	OwnerMenu->PlayMenuSound_Implementation(1);
	OwnerMenu->NavigateToMenu(FName("Equip"), CharacterWidget->GetCharacterName());
}
