// Source code implementation by Ephraim Gbadebo.


#include "FFYActionOptionWidget.h"

#include "FFYPartyMemberOptionWidget.h"
#include "FFY/FFYAction.h"
#include "FFY/FFYGameInstance.h"
#include "FFY/Widgets/FFYMenuWidget.h"
#include "HAL/MallocBinned2.h"

void UFFYActionOptionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UFFYActionOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFFYActionOptionWidget::InitializeOption(AFFYAction* ActionRef, int UIndex)
{
	UserIndex = UIndex;
	
	if (ActionRef)
	{
		Action = ActionRef;
		ActionNameBlock->SetText(FText::FromName(Action->Label));

		if (Action->MPCost > 0)
		{
			FText Value;
			FTextFormat FMT = FTextFormat::FromString("{Value} MP");
			FFormatNamedArguments Args;
			Args.Add("Value", Action->MPCost);
			Value = FText::Format(FMT, Args);
			if (MPCostBlock)
			{
				MPCostBlock->SetText(Value);
			}
		}
		if (Action->HPCost > 0)
		{
			FText Value;
			FTextFormat FMT = FTextFormat::FromString("{Value} HP");
			FFormatNamedArguments Args;
			Args.Add("Value", Action->HPCost);
			Value = FText::Format(FMT, Args);
			if (HPCostBlock)
			{
				HPCostBlock->SetText(Value);
			}
		}
	}
	
}

void UFFYActionOptionWidget::SetIsDisabled(bool Value)
{
	Super::SetIsDisabled(Value);
	if (ActionNameBlock)
	{
		FLinearColor InColor = bIsDisabled ? FLinearColor::Gray : FLinearColor::White;
		ActionNameBlock->SetColorAndOpacity(FSlateColor(InColor));
	}
}

FText UFFYActionOptionWidget::GetDescription()
{
	return (Action != nullptr) ? Action->Description : FText::GetEmpty();
}

void UFFYActionOptionWidget::OnSelected()
{
	Super::OnSelected();
}

void UFFYActionOptionWidget::OnUnselected()
{
	Super::OnUnselected();
}

void UFFYActionOptionWidget::PerformAction()
{
	if ( bIsDisabled || OwnerMenu->GetCurrentMenuMode() != EMenuMode::NONE )
	{
		return;
	}
	
	OwnerMenu->StartSelection_Implementation(this, Action->TargetType);
}

void UFFYActionOptionWidget::ContextAction_Implementation(UFFYPartyMemberOptionWidget* CharacterWidget, bool SelectAll)
{
	FName CharacterName = CharacterWidget->GetCharacterName();
	
	UFFYGameInstance* GameInstance = Cast<UFFYGameInstance>(GetWorld()->GetGameInstance());
	if (CharacterName.IsValid() && GameInstance)
	{
		//Get index of FPartySlot since directly referencing the struct from FPartySlot ref doesn't update
		TArray<FPartySlot>& Party = GameInstance->GetParty();
		for (int i = 0; i < Party.Num(); i++)
		{
			if (Party[i].PartyCharacterData.CharacterName == CharacterName || SelectAll)
			{
				FBattleCharacterData& User = Party[UserIndex].PartyCharacterData;

				Action->ExecuteAction(User, Party[i].PartyCharacterData);
				//GameInstance->ItemManager->Use(GameInstance->Inventory[Index], Party[i].PartyCharacterData);

				bool Exit = !(Action->CanCast(User));
				if (Exit)
				{
					OwnerMenu->EndSelection_Implementation();
					OwnerMenu->ContextRefresh_Implementation(UserIndex);
				}
				else
				{
					if (!SelectAll)
					{
						CharacterWidget->InitializeOption(Party[i]);
						OwnerMenu->ContextRefresh_Implementation(UserIndex);
					}
				}
			}
		}
		if (SelectAll)
		{
			UFFYMenuWidget* SelectionMenu = CharacterWidget->GetOwnerMenu();
			if (SelectionMenu)
			{
				SelectionMenu->Refresh();
				OwnerMenu->ContextRefresh_Implementation(UserIndex);
			}
		}
		
	}
}
