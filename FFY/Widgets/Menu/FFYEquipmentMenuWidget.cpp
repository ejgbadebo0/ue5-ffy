// Source code implementation by Ephraim Gbadebo.


#include "FFYEquipmentMenuWidget.h"

#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "Components/WrapBox.h"
#include "FFY/FFYDataEnums.h"
#include "FFY/FFYGameInstance.h"
#include "FFY/FFYItemDataStructs.h"
#include "FFY/Widgets/Option/FFYEquipmentItemOptionWidget.h"
#include "FFY/Widgets/Option/FFYSlotEquipOptionWidget.h"

void UFFYEquipmentMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Construct...");
	if (WeaponSlotWidget)
	{
		WeaponSlotWidget->SetOwnerMenu(this);
		WeaponSlotWidget->EquipmentSlot = EEquipmentSlot::WEAPON;
	}
	if (ShieldSlotWidget)
	{
		ShieldSlotWidget->SetOwnerMenu(this);
		ShieldSlotWidget->EquipmentSlot = EEquipmentSlot::SHIELD;
	}
	if (ArmorSlotWidget)
	{
		ArmorSlotWidget->SetOwnerMenu(this);
		ArmorSlotWidget->EquipmentSlot = EEquipmentSlot::ARMOR;
	}
	if (AccessorySlotWidget)
	{
		AccessorySlotWidget->SetOwnerMenu(this);
		AccessorySlotWidget->EquipmentSlot = EEquipmentSlot::ACCESSORY;
	}
	
}

void UFFYEquipmentMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UFFYEquipmentMenuWidget::Refresh()
{
	Super::Refresh();
	//Clear Delta Text values
	ClearDeltaValues();
	
}

void UFFYEquipmentMenuWidget::ClearDeltaValues() const
{
	DeltaSTR->SetText(FText::GetEmpty());
	DeltaCON->SetText(FText::GetEmpty());
	DeltaMAG->SetText(FText::GetEmpty());
	DeltaSPR->SetText(FText::GetEmpty());
	DeltaPRS->SetText(FText::GetEmpty());
	DeltaDEX->SetText(FText::GetEmpty());
	DeltaLUC->SetText(FText::GetEmpty());
}

/*
 * Allow for Unequiping current equipment item.
 */
void UFFYEquipmentMenuWidget::ConstructUnequipOption(EEquipmentClass EquipmentClass, EEquipmentSlot EquipmentSlot)
{
	UFFYGameInstance* GameInstance = Cast<UFFYGameInstance>(GetGameInstance());

	if (GameInstance)
	{
		// Set data
		auto ToAdd = CreateWidget<UFFYEquipmentItemOptionWidget>(this, ItemButtonClass);
		if (ToAdd)
		{
			FItemData UnequipItem = FItemData();
			UnequipItem.ItemName = FName("(Unequip)");
			UnequipItem.EquipmentData.EquipmentSlot = EquipmentSlot;
			
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Turquoise, "ConstructWidget Successful!");
			ToAdd->InitializeOption(UnequipItem);
			ToAdd->SetOwnerMenu(this);
			Options.Add(ToAdd);
			if (WrapBox)
			{
				WrapBox->AddChildToWrapBox(ToAdd);
			}
		}
	}

}

void UFFYEquipmentMenuWidget::EndSelection_Implementation()
{
	//Super::EndSelection_Implementation();
	if (CurrentMenuMode == EMenuMode::SELECTING)
	{
		CurrentOption->Flicker_Implementation(false);
		Refresh();
		SetMenuMode(EMenuMode::NONE);
	}
}

void UFFYEquipmentMenuWidget::StartSelection_Implementation(UFFYOptionWidget* SelectedOption, ETargetType SelectedTargetType)
{
	//Super::StartSelection_Implementation(SelectedOption);
	
	SetCurrentOption(SelectedOption);
	CurrentOption->Flicker_Implementation(true);
	SetMenuMode(EMenuMode::SELECTING);
	// rest done by LoadEquipment, as not using a party member widget
}

void UFFYEquipmentMenuWidget::LoadContext_Implementation(FName ContextName)
{
	Super::LoadContext_Implementation(ContextName);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "LoadingContext...");
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
			UTexture2D* Image = PartySlot.PartyCharacterData.Portrait;
			if (Image)
			{
				Portrait->SetBrushFromTexture(Image);
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

			if (WeaponSlotWidget) //check if the buttons have been initialized
			{
				WeaponSlotWidget->SetEquipmentName(PartySlot.PartyCharacterData.Equipment.Weapon.ItemName);
				WeaponSlotWidget->EquipmentClass = PartySlot.PartyCharacterData.EquipmentClass;

				ShieldSlotWidget->SetEquipmentName(PartySlot.PartyCharacterData.Equipment.Shield.ItemName);
				ShieldSlotWidget->EquipmentClass = PartySlot.PartyCharacterData.EquipmentClass;

				ArmorSlotWidget->SetEquipmentName(PartySlot.PartyCharacterData.Equipment.Armor.ItemName);
				ArmorSlotWidget->EquipmentClass = PartySlot.PartyCharacterData.EquipmentClass;

				AccessorySlotWidget->SetEquipmentName(PartySlot.PartyCharacterData.Equipment.Accessory.ItemName);
				AccessorySlotWidget->EquipmentClass = PartySlot.PartyCharacterData.EquipmentClass;
			}
		}
	}
}
		

void UFFYEquipmentMenuWidget::EquipmentItemHovered_Implementation(UFFYEquipmentItemOptionWidget* SelectedItemWidget)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, "EquipmentItemHovered_Implementation" );
	PlayMenuSound_Implementation(0);
	DescriptionTextBlock->SetText(SelectedItemWidget->GetDescription());
	
	UFFYGameInstance* GameInstance = Cast<UFFYGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance)
	{
		FBattleCharacterData& PartyMember = GameInstance->GetParty()[ContextIndex].PartyCharacterData;
		FEquipmentData CompareData = GameInstance->ItemManager->Compare(SelectedItemWidget->InventoryItem, PartyMember);

		if (!(CompareData.Strength == PartyMember.Strength))
		{
			FText Value;
			FTextFormat FMT = FTextFormat::FromString("-> {Value}");
			FFormatNamedArguments Args;
			Args.Add("Value", CompareData.Strength);
			Value = FText::Format(FMT, Args);
			DeltaSTR->SetColorAndOpacity((CompareData.Strength > PartyMember.Strength) ? FColor::Green : FColor::Red);
			DeltaSTR->SetText(Value);
		}

		if (!(CompareData.Constitution == PartyMember.Constitution))
		{
			FText Value;
			FTextFormat FMT = FTextFormat::FromString("-> {Value}");
			FFormatNamedArguments Args;
			Args.Add("Value", CompareData.Constitution);
			Value = FText::Format(FMT, Args);
			DeltaCON->SetColorAndOpacity((CompareData.Constitution > PartyMember.Constitution) ? FColor::Green : FColor::Red);
			DeltaCON->SetText(Value);
		}

		if (!(CompareData.Magic == PartyMember.Magic))
		{
			FText Value;
			FTextFormat FMT = FTextFormat::FromString("-> {Value}");
			FFormatNamedArguments Args;
			Args.Add("Value", CompareData.Magic);
			Value = FText::Format(FMT, Args);
			DeltaMAG->SetColorAndOpacity((CompareData.Magic > PartyMember.Magic) ? FColor::Green : FColor::Red);
			DeltaMAG->SetText(Value);
		}
		if (!(CompareData.Spirit == PartyMember.Spirit))
		{
			FText Value;
			FTextFormat FMT = FTextFormat::FromString("-> {Value}");
			FFormatNamedArguments Args;
			Args.Add("Value", CompareData.Spirit);
			Value = FText::Format(FMT, Args);
			DeltaSPR->SetColorAndOpacity((CompareData.Spirit > PartyMember.Spirit) ? FColor::Green : FColor::Red);
			DeltaSPR->SetText(Value);
		}
		if (!(CompareData.Precision == PartyMember.Precision))
		{
			FText Value;
			FTextFormat FMT = FTextFormat::FromString("-> {Value}");
			FFormatNamedArguments Args;
			Args.Add("Value", CompareData.Precision);
			Value = FText::Format(FMT, Args);
			DeltaPRS->SetColorAndOpacity((CompareData.Precision > PartyMember.Precision) ? FColor::Green : FColor::Red);
			DeltaPRS->SetText(Value);
		}
		if (!(CompareData.Dexterity == PartyMember.Dexterity))
		{
			FText Value;
			FTextFormat FMT = FTextFormat::FromString("-> {Value}");
			FFormatNamedArguments Args;
			Args.Add("Value", CompareData.Dexterity);
			Value = FText::Format(FMT, Args);
			DeltaDEX->SetColorAndOpacity((CompareData.Dexterity > PartyMember.Dexterity) ? FColor::Green : FColor::Red);
			DeltaDEX->SetText(Value);
		}
		if (!(CompareData.Luck == PartyMember.Luck))
		{
			FText Value;
			FTextFormat FMT = FTextFormat::FromString("-> {Value}");
			FFormatNamedArguments Args;
			Args.Add("Value", CompareData.Luck);
			Value = FText::Format(FMT, Args);
			DeltaLUC->SetColorAndOpacity((CompareData.Luck > PartyMember.Luck) ? FColor::Green : FColor::Red);
			DeltaLUC->SetText(Value);
		}
	}
	
	
}

void UFFYEquipmentMenuWidget::EquipmentItemSelected_Implementation(UFFYEquipmentItemOptionWidget* SelectedItemWidget)
{
	UFFYGameInstance* GameInstance = Cast<UFFYGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance)
	{
		if (SelectedItemWidget->InventoryItem.ItemName == FName("(Unequip)"))
		{
			GameInstance->ItemManager->Unequip(SelectedItemWidget->InventoryItem.EquipmentData.EquipmentSlot, GameInstance->GetParty()[ContextIndex].PartyCharacterData);
			PlayMenuSound_Implementation(4);
		}
		else
		{
			int Index = GameInstance->FindInventoryItemIndex(SelectedItemWidget->InventoryItem.ID);
			GameInstance->ItemManager->Equip(GameInstance->Inventory[Index], GameInstance->GetParty()[ContextIndex].PartyCharacterData);
			PlayMenuSound_Implementation(3);
		}
	}
	EndSelection_Implementation();
	CycleContextIndex(0);
	
}

void UFFYEquipmentMenuWidget::EquipmentItemUnhovered_Implementation(UFFYEquipmentItemOptionWidget* SelectedItemWidget)
{
	ClearDeltaValues();
}

void UFFYEquipmentMenuWidget::LoadEquipmentItems_Implementation(EEquipmentClass EquipmentClass, EEquipmentSlot EquipmentSlot)
{

	UFFYGameInstance* GameInstance = Cast<UFFYGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance)
	{
		//Put Unequip option at the top of the list
		ConstructUnequipOption(EquipmentClass, EquipmentSlot);
		
		for (auto i : GameInstance->Inventory)
		{
			if (i.ID.IsValid() && i.bIsEquipable) //Filter equipment based on slot/class
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Turquoise, "Equipable");
				if (i.EquipmentData.EquipmentClass == EquipmentClass && i.EquipmentData.EquipmentSlot == EquipmentSlot)
				{
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Turquoise, "MatchSlot/Class");
					auto ToAdd = CreateWidget<UFFYEquipmentItemOptionWidget>(this, ItemButtonClass);
					if (ToAdd)
					{
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Turquoise, "ConstructWidget Successful!");
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
}
