// Source code implementation by Ephraim Gbadebo.


#include "FFYMagicMenuWidget.h"

#include "FFY/Widgets/Option/FFYActionOptionWidget.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "Components/WrapBox.h"
#include "FFY/FFYAction.h"
#include "FFY/FFYActionContainer.h"
#include "FFY/FFYGameInstance.h"

void UFFYMagicMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFFYMagicMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UFFYMagicMenuWidget::LoadContext_Implementation(FName ContextName)
{
	Refresh();

	UFFYGameInstance* GameInstance = Cast<UFFYGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance)
	{
		FPartySlot PartySlot;
		bool Result = false;
		
		TArray<FPartySlot>& Party = GameInstance->GetParty();
		for (int i = 0; i < Party.Num(); i++)
		{
			if (Party[i].PartyCharacterData.CharacterName == ContextName)
			{
				PartySlot = Party[i];
				
				Result = true;
				ContextIndex = i;
			}
		}
		if (Result) //Has a valid slot, Load widget info
		{
			UTexture2D** Image = PortraitTextureCache.Find(PartySlot.PartyCharacterData.CharacterName);
			if (Image)
			{
				Portrait->SetBrushFromTexture(*Image);
			}
	
			CharacterName->SetText(FText::FromName(PartySlot.PartyCharacterData.CharacterName));
			LevelValue->SetText(FText::AsNumber(PartySlot.PartyCharacterData.LV));
			HPValue->SetText(FText::AsNumber(PartySlot.PartyCharacterData.HP));
			MaxHPValue->SetText(FText::AsNumber(PartySlot.PartyCharacterData.MaxHP));
			MPValue->SetText(FText::AsNumber(PartySlot.PartyCharacterData.MP));
			MaxMPValue->SetText(FText::AsNumber(PartySlot.PartyCharacterData.MaxMP));
			EXPValue->SetText(FText::AsNumber(PartySlot.PartyCharacterData.EXP));
			ReqEXPValue->SetText(FText::AsNumber(PartySlot.PartyCharacterData.ReqEXP));

			
			for (auto i : GameInstance->PartyActions[ContextIndex]->Magic)
			{
				if (i)
				{

					auto ToAdd = CreateWidget<UFFYActionOptionWidget>(this, ActionOptionClass);
					if (ToAdd)
					{
						ToAdd->InitializeOption(i, ContextIndex);
						if (ToAdd->Action->Type < 1 || !(ToAdd->Action->CanCast(PartySlot.PartyCharacterData)))
						{
							ToAdd->SetIsDisabled(true);
						}
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
}

UFFYSelectPartyMemberWidget* UFFYMagicMenuWidget::GetSelectionWidget_Implementation()
{
	return SelectionWidget;
}

void UFFYMagicMenuWidget::ContextRefresh_Implementation(int Index)
{
	UFFYGameInstance* GameInstance = Cast<UFFYGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance)
	{
		FPartySlot PartySlot = GameInstance->GetParty()[Index];

		HPValue->SetText(FText::AsNumber(PartySlot.PartyCharacterData.HP));
		MaxHPValue->SetText(FText::AsNumber(PartySlot.PartyCharacterData.MaxHP));
		MPValue->SetText(FText::AsNumber(PartySlot.PartyCharacterData.MP));
		MaxMPValue->SetText(FText::AsNumber(PartySlot.PartyCharacterData.MaxMP));
	}
}

void UFFYMagicMenuWidget::Refresh()
{
	Super::Refresh();
}
