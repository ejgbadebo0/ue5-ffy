// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYSelectPartyMemberWidget.h"

#include "Components/WrapBox.h"
#include "FFY/FFYGameInstance.h"
#include "FFY/Widgets/Option/FFYPartyMemberOptionWidget.h"

void UFFYSelectPartyMemberWidget::NativeConstruct()
{
	//Super::NativeConstruct();

	//if (!bIsInitialized)
	//{
	Refresh();
		//bIsInitialized = true;
	//}
}

void UFFYSelectPartyMemberWidget::NativeDestruct()
{
	Super::NativeDestruct();
	ClearOptions();
	if (bSelectAll)
	{
		SetSelectAll(false);
		bCanSelectAll = false;
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Purple, TEXT("SELECTPARTY: DESTRUCT"));
}

void UFFYSelectPartyMemberWidget::ClearOptions()
{
	Super::ClearOptions();
}

void UFFYSelectPartyMemberWidget::Refresh()
{
	Super::Refresh();

	UFFYGameInstance* GameInstance = Cast<UFFYGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		//get party member data
		for (FPartySlot i: GameInstance->GetParty())
		{
			auto ToAdd = CreateWidget<UFFYPartyMemberOptionWidget>(this, PartyButtonClass);
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

void UFFYSelectPartyMemberWidget::SetSelectAll(bool Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Silver, FString::Printf(TEXT("%s: SELECT ALL - %s"), *this->GetDisplayLabel(), (Value) ? "true" : "false"));

	if (bCanSelectAll)
	{
		bSelectAll = Value;

		for (auto o : Options)
		{
			if (o)
			{
				o->Flicker(bSelectAll);
			}
		}
	}
}

void UFFYSelectPartyMemberWidget::PartyMemberSelect_Implementation(UFFYPartyMemberOptionWidget* CharacterWidget)
{
	OnPartyMemberSelected.Broadcast(CharacterWidget, bSelectAll);
}

