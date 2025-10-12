// Source code implementation by Ephraim Gbadebo.


#include "FFYInventoryItemOptionWidget.h"
#include "FFYPartyMemberOptionWidget.h"

#include "FFY/FFYItem.h"
#include "FFY/Widgets/FFYMenuWidget.h"

void UFFYInventoryItemOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFFYInventoryItemOptionWidget::OnSelected()
{
	Super::OnSelected();
}

void UFFYInventoryItemOptionWidget::OnUnselected()
{
	Super::OnUnselected();
}

void UFFYInventoryItemOptionWidget::PerformAction()
{
	if ( bIsDisabled || OwnerMenu->GetCurrentMenuMode() != EMenuMode::NONE )
	{
		return;
	}

	OwnerMenu->StartSelection_Implementation(this, InventoryItem.TargetType);
	
}

void UFFYInventoryItemOptionWidget::SetIsDisabled(bool Value)
{
	Super::SetIsDisabled(Value);
	if (InventoryItemNameBlock)
	{
		FLinearColor InColor = bIsDisabled ? FLinearColor::Gray : FLinearColor::White;
		InventoryItemNameBlock->SetColorAndOpacity(FSlateColor(InColor));
	}

	
}

void UFFYInventoryItemOptionWidget::InitializeOption(const FItemData& InventoryItemReference)
{
	if (InventoryItemReference.ID.IsValid())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Inventory Item Button CONSTRUCT");
		InventoryItem = InventoryItemReference;

		InventoryItemNameBlock->SetText(FText::FromName(InventoryItem.ItemName));
		InventoryItemAmountBlock->SetText( (InventoryItem.Amount > 1) ? FText::AsNumber(InventoryItem.Amount) : FText::GetEmpty());
		

		SetIsDisabled(!InventoryItem.bIsConsumable);
		
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Inventory Item  is NULL");
	}
}

void UFFYInventoryItemOptionWidget::ContextAction_Implementation(UFFYPartyMemberOptionWidget* CharacterWidget, bool SelectAll)
{
	FName CharacterName = CharacterWidget->GetCharacterName();
	UFFYGameInstance* GameInstance = Cast<UFFYGameInstance>(GetWorld()->GetGameInstance());
	OwnerMenu->PlayMenuSound_Implementation(3);
	if (CharacterName.IsValid() && GameInstance)
	{
		//Get index of FPartySlot since directly referencing the struct from FPartySlot ref doesn't update
		TArray<FPartySlot>& Party = GameInstance->GetParty();
		for (int i = 0; i < Party.Num(); i++)
		{
			if (Party[i].PartyCharacterData.CharacterName == CharacterName || SelectAll)
			{
				int Index = GameInstance->FindInventoryItemIndex(InventoryItem.ID);
				GameInstance->ItemManager->Use(GameInstance->Inventory[Index], Party[i].PartyCharacterData, (i == Party.Num()-1) || !SelectAll);

				int Used = GameInstance->FindInventoryItemIndex(InventoryItem.ID);
				if (Used == -1)
				{
					OwnerMenu->EndSelection_Implementation();
					OwnerMenu->Refresh();
				}
				else
				{
					InitializeOption(GameInstance->Inventory[Index]);
					CharacterWidget->InitializeOption(Party[i]);
				}
			}
		}
		
	}
}
