// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYGameInstance.h"

//ext includes
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "FFYCharacter.h"
#include "Blueprint/UserWidget.h"
#include "FFYDataStructures.h"
#include "FFYItem.h"
#include "FFYAction.h"
#include "FFYActionContainer.h"
#include "FFYBattleCharacter.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "UniversalObjectLocators/UniversalObjectLocatorUtils.h"
//-------
void UFFYGameInstance::Init()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("GameInstance: Init"));

	//AFFYGameMode* GameMode = Cast<AFFYGameMode>(GetWorld()->GetAuthGameMode());

	GetWorld()->GetTimerManager().SetTimer(PlayTimeHandle, this, &UFFYGameInstance::ElapsePlayTime, 1.f, true, -1.f);
	
	TArray<FBattleCharacterData*> CharacterData;
	PartyTableHandle.DataTable->GetAllRows("", CharacterData);

	for (const auto i : CharacterData)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("PartyEntry"));
		FBattleCharacterData NewData;
		NewData.BattleCharacterClass = i->BattleCharacterClass;
		NewData.EquipmentClass = i->EquipmentClass;
		NewData.CharacterName = i->CharacterName;
		NewData.LV = i->LV;
		NewData.AP = i->AP;
		NewData.MaxHP = i->MaxHP;
		NewData.HP = i->HP;
		NewData.MaxMP = i->MP;
		NewData.MP = i->MP;
		NewData.LP = i->LP;
		NewData.Strength = i->Strength;
		NewData.Constitution = i->Constitution;
		NewData.Magic = i->Magic;
		NewData.Spirit = i->Spirit;
		NewData.Precision = i->Precision;
		NewData.Dexterity = i->Dexterity;
		NewData.Luck = i->Luck;
		NewData.EXP = i->EXP;
		NewData.ReqEXP = i->ReqEXP;
		NewData.StatusEffects = i->StatusEffects;
		NewData.Weaknesses = i->Weaknesses;
		NewData.Resistances = i->Resistances;
		NewData.Catalysts = i->Catalysts;
		NewData.Immunities = i->Immunities;
		
		FPartySlot NewSlot;
		NewSlot.PartyCharacterData = NewData;
		FClassAbilities* AbilityRow = ClassTableHandle.DataTable->FindRow<FClassAbilities>(NewData.CharacterName, "", true);

		UFFYActionContainer* NewActionContainer = NewObject<UFFYActionContainer>(this, UFFYActionContainer::StaticClass());
		for (auto a : AbilityRow->Abilities)
		{
			NewSlot.PartyCharacterAbilities.Emplace(a);
			
			AFFYAction* ActionInstance = NewObject<AFFYAction>(this, a);
			if (ActionInstance && NewActionContainer)
			{
				NewActionContainer->AddAction(ActionInstance);
			}
		}
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("PartyEntry"));
		Party.Emplace(NewSlot);
		PartyActions.Emplace(NewActionContainer);
		
	}

	InitializeBattleMap();

	//INVENTORY
	//AddToInventory(UFFYPotionItem::StaticClass(), 3);
	FItemData* StarterWeapon1 = ItemsTableHandle.DataTable->FindRow<FItemData>(FName("8"), "", true);
	FItemData* StarterWeapon2 = ItemsTableHandle.DataTable->FindRow<FItemData>(FName("9"), "", true);
	FItemData* StarterWeapon3 = ItemsTableHandle.DataTable->FindRow<FItemData>(FName("10"), "", true);
	FItemData* StarterShield = ItemsTableHandle.DataTable->FindRow<FItemData>(FName("11"), "", true);
	if (StarterWeapon1)
	{
		//add default equipment
		AddToInventory(*StarterWeapon1, 2);
		AddToInventory(*StarterWeapon2, 1);
		AddToInventory(*StarterWeapon3, 1);
		AddToInventory(*StarterShield, 1);
		
	}
	
	ItemManager = NewObject<UFFYItem>(this, UFFYItem::StaticClass());

	if (ItemManager)
	{
		//try to equip starter equipment
		ItemManager->Equip(Inventory[FindInventoryItemIndex(StarterWeapon1->ID)], GetParty()[0].PartyCharacterData);
		ItemManager->Equip(Inventory[FindInventoryItemIndex(StarterShield->ID)], GetParty()[0].PartyCharacterData);
		ItemManager->Equip(Inventory[FindInventoryItemIndex(StarterWeapon2->ID)], GetParty()[2].PartyCharacterData);
		ItemManager->Equip(Inventory[FindInventoryItemIndex(StarterWeapon3->ID)], GetParty()[1].PartyCharacterData);
	}

	//bind delegates
}

void UFFYGameInstance::Shutdown()
{
	GetWorld()->GetTimerManager().ClearTimer(PlayTimeHandle);
	Super::Shutdown();
}

void UFFYGameInstance::EvaluateEncounter(float BattleCounter)
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Counter: %d"), BattleCounter));
	if (BattleCounter >= BattleLevelMapInfo.EncounterFloor)
	{
		int32 Value = FMath::RandRange(0, 100);
		if (Value > 70 && !bIsLoadingMap)
		{
			bIsLoadingMap = true;
			AFFYCharacter* PlayerCharacter = Cast<AFFYCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			PlayerPreBattleInfo.LastPlayerTransform = PlayerCharacter->GetTransform();
			PlayerPreBattleInfo.LoadTransform = true;
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("ENCOUNTER TRIGGERED!!!"));

			StartEncounter();
		}
	}
}

void UFFYGameInstance::StartEncounter()
{
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (CameraManager)
	{
		AActor* ViewTarget = CameraManager->GetViewTarget();
		if (ViewTarget)
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.0f);
			
			IFFYCameraControls* CallTarget = Cast<IFFYCameraControls>(ViewTarget);
			if (CallTarget)
			{
				CallTarget->Execute_FrameCapture(ViewTarget);
			}
			StartTransition_Implementation("Battle");
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("START ENCOUNTER VALID"));	

			UGameplayStatics::OpenLevel(GetWorld(), BattleLevelMapInfo.BattleMapName, true);
			bIsLoadingMap = false;
		}
	}
	
}

void UFFYGameInstance::EndEncounter()
{
	StartTransition_Implementation("Pause");

	UGameplayStatics::OpenLevel(GetWorld(), PlayerPreBattleInfo.CurrentLevelName, true);
	
}


void UFFYGameInstance::ElapsePlayTime()
{
	Seconds++;
	if (Seconds >= 60)
	{
		Seconds = 0;
		Minutes++;
		if (Minutes >= 60)
		{
			Minutes = 0;
			Hours++;
		}
	}
}

int UFFYGameInstance::FindInventoryItemIndex(FName ItemID)
{
	int Index = -1;
	
	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		if (Inventory[i].ID == ItemID)
		{
			Index = i;
		}
	}
	return Index;
}

/*
 * For debugging inventory.
 */
void UFFYGameInstance::PrintInventory()
{
	for (auto i : Inventory)
	{
		FString STR = i.ItemName.ToString();
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("Item: %s, Amount: %d"), *STR, i.Amount ));
	}
}

/*
 * Takes a reference to the item in the Inventory array
 */
void UFFYGameInstance::RemoveFromInventory(FItemData InventoryItem)
{
	int Index = FindInventoryItemIndex(InventoryItem.ID);

	if (Index >= 0)
	{
		Inventory.RemoveAt(Index);
		OnInventoryUpdated.Broadcast();
	}
}

/*
 * Takes an item and amount of an item to add to Inventory array. 
 */
void UFFYGameInstance::AddToInventory(FItemData InventoryItem, int32 Amount)
{
	
	int Index = FindInventoryItemIndex(InventoryItem.ID);

	if (Index >= 0)
	{
		Inventory[Index].Amount += Amount;
	}
	else
	{
		//make sure is a valid item we're trying to add
		if (InventoryItem.ID == NAME_None)
		{
			return;
		}
		//didn't find item in Inventory, so construct a new item
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("Item constructed, adding"));
		int AddAmount = (InventoryItem.Amount > 0) ? Amount : 1; //make sure to initialize Amount to at least 1
		InventoryItem.Amount = AddAmount;
		Inventory.Emplace(InventoryItem);
	}
	//PrintInventory();
	OnInventoryUpdated.Broadcast();
}

void UFFYGameInstance::InitializeBattleMap()
{
	FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
	PlayerPreBattleInfo.CurrentLevelName = FName(*LevelName);

	FString Output = FString::Printf(TEXT("GameInstance: CurrentLevelName: %s"), *PlayerPreBattleInfo.CurrentLevelName.ToString());
	GEngine->AddOnScreenDebugMessage(-1, 35.0f, FColor::Black, Output);
	
	FBattleLevelMap* BattleMapInfo = BattleMapTableHandle.DataTable->FindRow<FBattleLevelMap>(PlayerPreBattleInfo.CurrentLevelName,"", true);
	
	if (BattleMapInfo)
	{
		BattleLevelMapInfo.EncounterFloor = BattleMapInfo->EncounterFloor;
		BattleLevelMapInfo.BattleMapName = BattleMapInfo->BattleMapName;
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("BATTLEMAP VALID"));
		bHasEncounters = (BattleMapInfo->EncounterFloor > 0) ? true : false;
		AFFYCharacter* PlayerCharacter = Cast<AFFYCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (PlayerCharacter)
		{
			PlayerCharacter->SetHasEncounters(bHasEncounters);
		}
	}
	else
	{
		bHasEncounters = false;
	}
}


void UFFYGameInstance::StartTransition_Implementation(FName Signature)
{
	IFFYTransitions::StartTransition_Implementation(Signature);

	auto TransitionClass = Transitions.Find(Signature);
	if (TransitionClass)
	{
		TransitionWidget = CreateWidget<UUserWidget>(this, *TransitionClass);
		if (TransitionWidget)
		{
			GetGameViewportClient()->AddViewportWidgetContent(TransitionWidget->TakeWidget());
			//TransitionWidget->AddToViewport();
		}
	}

}

void UFFYGameInstance::EndTransition_Implementation(FName Signature)
{
	IFFYTransitions::EndTransition_Implementation(Signature);
	GetGameViewportClient()->RemoveViewportWidgetContent(TransitionWidget->TakeWidget());
	if (TransitionWidget)
	{
		TransitionWidget->RemoveFromParent();
	}
}

void UFFYGameInstance::UpdatePartyMemberStats_Implementation(FBattleCharacterData CharacterData)
{
	
	for (auto &i : Party)
	{
		if (i.PartyCharacterData.CharacterName == CharacterData.CharacterName)
		{
			//Found party member
			i.PartyCharacterData = CharacterData;
			break;
		}
	}
	
}

void UFFYGameInstance::UseInventoryItem_Implementation(FName ID, AFFYBattleCharacter* Source,
                                                       AFFYBattleCharacter* Target)
{
	int Index = FindInventoryItemIndex(ID);
	if (Index > -1)
	{
		ItemManager->BattleUse(Inventory[Index], Source, Target);
	}
}
