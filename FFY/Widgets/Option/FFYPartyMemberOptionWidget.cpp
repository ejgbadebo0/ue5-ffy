// Source code implementation by Ephraim Gbadebo.


#include "FFYPartyMemberOptionWidget.h"

#include "SkeletalMeshAttributes.h"
#include "FFY/Widgets/FFYMenuWidget.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "FFY/FFYWidgetEvents.h"

void UFFYPartyMemberOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (OwnerMenu)
	{
		if (OwnerMenu->GetSelectAll_Implementation())
		{
			Flicker(true);
		}
	}
}

void UFFYPartyMemberOptionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UFFYPartyMemberOptionWidget::InitializeOption(FPartySlot& PartySlot)
{
	
	UTexture2D* Image = PartySlot.PartyCharacterData.Portrait;
	if (Image)
	{
		Portrait->SetBrushFromTexture(Image);
	}
	
	CharacterName->SetText(FText::FromName(PartySlot.PartyCharacterData.CharacterName));
	LevelValue->SetText(FText::AsNumber(PartySlot.PartyCharacterData.LV));
	FText HPText = FText::AsNumber(PartySlot.PartyCharacterData.HP);
	if ((PartySlot.PartyCharacterData.HP/PartySlot.PartyCharacterData.MaxHP) < 0.25f)
	{
		FText Status = (PartySlot.PartyCharacterData.HP <= 0.f) ? FText::FromString("<KO>") : FText::FromString("<Weak>");
		FTextFormat FMT = FTextFormat::FromString("{SFMT}{Value}</>");
		FFormatNamedArguments Args;
		Args.Add("SFMT", Status);
		Args.Add("Value", HPText);
		HPText = FText::Format(FMT, Args);
		HPValue->SetText(HPText);
	}
	else
	{
		HPValue->SetText(HPText);
	}
	MaxHPValue->SetText(FText::AsNumber(PartySlot.PartyCharacterData.MaxHP));
	MPValue->SetText(FText::AsNumber(PartySlot.PartyCharacterData.MP));
	MaxMPValue->SetText(FText::AsNumber(PartySlot.PartyCharacterData.MaxMP));
	EXPValue->SetText(FText::AsNumber(PartySlot.PartyCharacterData.EXP));
	ReqEXPValue->SetText(FText::AsNumber(PartySlot.PartyCharacterData.ReqEXP));
	
}

FName UFFYPartyMemberOptionWidget::GetCharacterName()
{
	if (CharacterName)
	{
		FText Text = CharacterName->GetText();
		FName Name = FName(Text.ToString());
		return Name;
	} else
	{
		return NAME_None;
	}
}

void UFFYPartyMemberOptionWidget::OnSelected()
{
	Super::OnSelected();
}

void UFFYPartyMemberOptionWidget::OnUnselected()
{
	Super::OnUnselected();
}

void UFFYPartyMemberOptionWidget::PerformAction()
{
	//Super::PerformAction();
	if (OwnerMenu)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, "PARTY MEMBER SELECTED.");
		IFFYWidgetEvents* MenuWidget = Cast<IFFYWidgetEvents>(OwnerMenu);
		if (MenuWidget)
		{
			MenuWidget->PartyMemberSelect_Implementation(this);
		}
	}
}
