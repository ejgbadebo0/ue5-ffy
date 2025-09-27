// Source code implementation by Ephraim Gbadebo.


#include "FFYMagicOptionWidget.h"

#include "FFYPartyMemberOptionWidget.h"
#include "FFY/FFYDataEnums.h"
#include "FFY/Widgets/FFYMenuWidget.h"

void UFFYMagicOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFFYMagicOptionWidget::OnSelected()
{
	Super::OnSelected();
}

void UFFYMagicOptionWidget::OnUnselected()
{
	Super::OnUnselected();
}


void UFFYMagicOptionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UFFYMagicOptionWidget::PerformAction()
{
	if ( bIsDisabled || OwnerMenu->GetCurrentMenuMode() != EMenuMode::NONE )
	{
		return;
	}
	OwnerMenu->StartSelection_Implementation(this, ETargetType::SINGLE);
	OwnerMenu->SetDescription(FText::FromString("Select a party member."));
}


void UFFYMagicOptionWidget::ContextAction_Implementation(UFFYPartyMemberOptionWidget* CharacterWidget, bool SelectAll)
{
	OwnerMenu->PlayMenuSound_Implementation(1);
	OwnerMenu->NavigateToMenu(FName("Magic"), CharacterWidget->GetCharacterName());
}