// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//ext includes
#include "FFYDataStructures.h"
//-----
#include "FFYBattleEvents.h"
#include "FFYCharacter.h"
#include "FFYTransitions.h"
#include "IDetailTreeNode.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "FFYGameInstance.generated.h"


class UFFYActionContainer;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

class AFFYAction;
class UFFYItem;

USTRUCT(BlueprintType)
struct FFY_API FPartySlot
{
	GENERATED_BODY();

	FBattleCharacterData PartyCharacterData;
	TArray<TSubclassOf<AFFYAction>> PartyCharacterAbilities;
	//TArray<AFFYAction*> PartyCharacterActions;
};

USTRUCT(BlueprintType)
struct FFY_API FClassAbilities : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AFFYBattleCharacter> BattleCharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AFFYAction>> Abilities;
};

USTRUCT(BlueprintType)
struct FFY_API FBattleLevelMap : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EncounterFloor = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BattleMapName = "M_Battle";
};

USTRUCT(BlueprintType)
struct FFY_API FPlayerPreBattleInfo
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CurrentLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool LoadTransform = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform LastPlayerTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LastSceneIndex = 0;
	
};
/**
 *  Main game class, holds party info/progress.
 */
UCLASS()
class FFY_API UFFYGameInstance : public UGameInstance, public IFFYTransitions, public IFFYBattleEvents
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FPartySlot> Party;

	UPROPERTY(EditAnywhere)
	int32 Gil;

	UPROPERTY(EditAnywhere)
	FText PlayTime;

public:
	//DELEGATES:
	FOnInventoryUpdated OnInventoryUpdated;
	//-----------

	
	//setup initial game variables
	virtual void Init() override;

	virtual void Shutdown() override;

	void EvaluateEncounter(float BattleCounter);

	void StartEncounter();

	void EndEncounter();

	UFUNCTION(BlueprintCallable)
	TArray<FPartySlot>& GetParty()
	{
		return Party;
	}

	UFUNCTION(BlueprintCallable)
	int32 GetGil()
	{
		return Gil;
	}

	UFUNCTION(BlueprintCallable)
	FText GetPlayTime()
	{
		FTextFormat FMT = FTextFormat::FromString("{Hours}:{Minutes}:{Seconds}");
		FFormatNamedArguments Args;
		FText SVal = (Seconds < 9) ? FText::FromString(FString::Printf(TEXT("0%d"), Seconds)) : FText::FromString(FString::FromInt(Seconds));
		FText MVal = (Minutes < 9) ? FText::FromString(FString::Printf(TEXT("0%d"), Minutes)) : FText::FromString(FString::FromInt(Minutes));
		FText HVal = (Hours < 9) ? FText::FromString(FString::Printf(TEXT("0%d"), Hours)) : FText::FromString(FString::FromInt(Hours));
		Args.Add("Hours",HVal);
		Args.Add("Minutes", MVal);
		Args.Add("Seconds", SVal );
		PlayTime = FText::Format(FMT, Args);
		return PlayTime;
	}
	/*
	 * Indexing should correspond to FPartySlots. 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Actions")
	TArray<UFFYActionContainer*> PartyActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Inventory")
	TArray<FItemData> Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Inventory")
	UFFYItem* ItemManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="DEBUG")
	UBlueprint* DebugBlueprint;
	
	FTimerHandle PlayTimeHandle;
	
	UFUNCTION(BlueprintCallable)
	void ElapsePlayTime();

	int Seconds = 0;
	int Minutes = 0;
	int Hours = 0;
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	int FindInventoryItemIndex(FName ItemID);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void PrintInventory();
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void RemoveFromInventory(FItemData InventoryItem);

	UFUNCTION(BlueprintCallable, Category="Inventory") 
	void AddToInventory(FItemData InventoryItem, int32 Amount);
	

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (RowType = "BattleCharacterData"))
	FDataTableRowHandle PartyTableHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Inventory")
	FDataTableRowHandle ItemsTableHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (RowType = "ClassAbilities"))
	FDataTableRowHandle ClassTableHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (RowType = "BattleLevelMap"))
	FDataTableRowHandle BattleMapTableHandle;

	UFUNCTION(BlueprintCallable)
	void InitializeBattleMap();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FBattleCharacterData> PartyTableData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<class AFFYAction>> StarterAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FPlayerPreBattleInfo PlayerPreBattleInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bHasEncounters = false;

	bool bIsLoadingMap = false;
	
	FBattleLevelMap BattleLevelMapInfo;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	TMap<FName, TSubclassOf<UUserWidget>> Transitions;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	UUserWidget* TransitionWidget;

	//INTERFACE: =================
	
	void StartTransition_Implementation(FName Signature) override;

	void EndTransition_Implementation(FName Signature) override;
	
	virtual void UpdatePartyMemberStats_Implementation(FBattleCharacterData CharacterData) override;

	virtual void
	UseInventoryItem_Implementation(FName ID, AFFYBattleCharacter* Source, AFFYBattleCharacter* Target) override;

	virtual FName GetItemName_Implementation(FName ID) override
	{
		FItemData* Item = ItemsTableHandle.DataTable->FindRow<FItemData>(ID, "", true);
		if (Item)
		{
			return Item->ItemName;
		}
		else
		{
			return NAME_None;
		}
	}

	virtual void EndBattle_Implementation() override
	{
		EndEncounter();
	}
};

