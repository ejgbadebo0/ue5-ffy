// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYStatusMenuWidget.h"

#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "FFY/FFYGameInstance.h"

void UFFYStatusMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFFYStatusMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UFFYStatusMenuWidget::Refresh()
{
	Super::Refresh();

	StatusEffectsBox->ClearChildren();
	WeaknessesBox->ClearChildren();
	ResistancesBox->ClearChildren();
	CatalystsBox->ClearChildren();
	ImmunitiesBox->ClearChildren();
	
}

UFFYSelectPartyMemberWidget* UFFYStatusMenuWidget::GetSelectionWidget_Implementation()
{
	return Super::GetSelectionWidget_Implementation();
}

void UFFYStatusMenuWidget::LoadContext_Implementation(FName ContextName)
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
			//Stats
			StrengthValue->SetText(FText::AsNumber(PartySlot.PartyCharacterData.Strength));
			ConstitutionValue->SetText(FText::AsNumber(PartySlot.PartyCharacterData.Constitution));
			MagicValue->SetText(FText::AsNumber(PartySlot.PartyCharacterData.Magic));
			SpiritValue->SetText(FText::AsNumber(PartySlot.PartyCharacterData.Spirit));
			PrecisionValue->SetText(FText::AsNumber(PartySlot.PartyCharacterData.Precision));
			DexterityValue->SetText(FText::AsNumber(PartySlot.PartyCharacterData.Dexterity));
			LuckValue->SetText(FText::AsNumber(PartySlot.PartyCharacterData.Luck));

			//Current Status Effects
			for (auto i : PartySlot.PartyCharacterData.StatusEffects)
			{
				FText SEText;
				UEnum::GetDisplayValueAsText(i, SEText);
				UTextBlock* SETextBlock = NewObject<UTextBlock>();
				if (i != PartySlot.PartyCharacterData.StatusEffects.Last())
				{
					FTextFormat FMT = FTextFormat::FromString("{Status}, ");
					FFormatNamedArguments Args;
					Args.Add("Status", SEText);
					SEText = FText::Format(FMT, Args);
				}
				SETextBlock->SetText(SEText);
				StatusEffectsBox->AddChildToHorizontalBox(SETextBlock);
			}
			//Weaknesses
			for (auto i : PartySlot.PartyCharacterData.Weaknesses)
			{
				FText WEText;
				UEnum::GetDisplayValueAsText(i, WEText);
				UTextBlock* WETextBlock = NewObject<UTextBlock>();
				if (i != PartySlot.PartyCharacterData.Weaknesses.Last())
				{
					FTextFormat FMT = FTextFormat::FromString("{Element}, ");
					FFormatNamedArguments Args;
					Args.Add("Element", WEText);
					WEText = FText::Format(FMT, Args);
				}
				WETextBlock->SetText(WEText);
				StatusEffectsBox->AddChildToHorizontalBox(WETextBlock);
			}
			//Resistances
			for (auto i : PartySlot.PartyCharacterData.Resistances)
			{
				FText RText;
				UEnum::GetDisplayValueAsText(i, RText);
				UTextBlock* RTextBlock = NewObject<UTextBlock>();
				if (i != PartySlot.PartyCharacterData.Resistances.Last())
				{
					FTextFormat FMT = FTextFormat::FromString("{Element}, ");
					FFormatNamedArguments Args;
					Args.Add("Element", RText);
					RText = FText::Format(FMT, Args);
				}
				RTextBlock->SetText(RText);
				StatusEffectsBox->AddChildToHorizontalBox(RTextBlock);
			}
			//Catalysts
			for (auto i : PartySlot.PartyCharacterData.Catalysts)
			{
				FText CText;
				UEnum::GetDisplayValueAsText(i, CText);
				UTextBlock* CTextBlock = NewObject<UTextBlock>();
				if (i != PartySlot.PartyCharacterData.Catalysts.Last())
				{
					FTextFormat FMT = FTextFormat::FromString("{Element}, ");
					FFormatNamedArguments Args;
					Args.Add("Element", CText);
					CText = FText::Format(FMT, Args);
				}
				CTextBlock->SetText(CText);
				StatusEffectsBox->AddChildToHorizontalBox(CTextBlock);
			}
			//Immunities
			for (auto i : PartySlot.PartyCharacterData.Immunities)
			{
				FText IText;
				UEnum::GetDisplayValueAsText(i, IText);
				UTextBlock* ITextBlock = NewObject<UTextBlock>();
				if (i != PartySlot.PartyCharacterData.Immunities.Last())
				{
					FTextFormat FMT = FTextFormat::FromString("{Status}, ");
					FFormatNamedArguments Args;
					Args.Add("Status", IText);
					IText = FText::Format(FMT, Args);
				}
				ITextBlock->SetText(IText);
				StatusEffectsBox->AddChildToHorizontalBox(ITextBlock);
			}
			
		}

		
	}
}
