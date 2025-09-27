// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"

//ext includes
#include "FFYDataStructures.h"
//-----
#include "FFYBattleEvents.h"
#include "FFYWidgetEvents.h"
#include "FFYCharacter.h"
#include "FFYSaveDataEvents.h"
#include "FFYTransitions.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "FFYGameInstance.generated.h"


class UFFYActionContainer;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayTimeUpdated, FText, PTText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWorldDamage, bool, bIsPoison);

class AFFYAction;
class UFFYItem;

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
UCLASS(BlueprintType, Blueprintable)
class FFY_API UFFYGameInstance : public UGameInstance, public IFFYTransitions, public IFFYBattleEvents, public IFFYWidgetEvents, public IFFYSaveDataEvents
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
	UPROPERTY(BlueprintAssignable)
	FOnPlayTimeUpdated OnPlayTimeUpdated;
	UPROPERTY(BlueprintAssignable)
	FOnWorldDamage OnWorldDamage;
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
		FText SVal = (Seconds < 10) ? FText::FromString(FString::Printf(TEXT("0%d"), Seconds)) : FText::FromString(FString::FromInt(Seconds));
		FText MVal = (Minutes < 10) ? FText::FromString(FString::Printf(TEXT("0%d"), Minutes)) : FText::FromString(FString::FromInt(Minutes));
		FText HVal = (Hours < 10) ? FText::FromString(FString::Printf(TEXT("0%d"), Hours)) : FText::FromString(FString::FromInt(Hours));
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Save")
	TMap<FName, uint8> ActorSaveData; 
	
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

	UFUNCTION(BlueprintCallable, Category=Save)
	void AddToSaveState(FName ItemID, uint8 State)
	{
		ActorSaveData.Add(ItemID, State);
	}


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

	virtual void AddFromDrop_Implementation(FName ItemID, int32 Amount) override
	{
		FItemData* Item = ItemsTableHandle.DataTable->FindRow<FItemData>(ItemID, "", true);
		if (Item)
		{
			AddToInventory(*Item, Amount);
		}
	}

	virtual void UpdatePlayTime_Implementation() override
	{
		ElapsePlayTime();
	}
	
	virtual bool HasSaveData_Implementation(FName ID) override
	{
		return ActorSaveData.Contains(ID);
	}

	virtual void SetSaveState_Implementation(FName ID, uint8 State) override
	{
		AddToSaveState(ID, State);
	}

	virtual uint8 GetSaveState_Implementation(FName ID) override
	{
		uint8 Result = *ActorSaveData.Find(ID);
	
		return Result;
	}
	
	virtual void EvaluateStatusEffects_Implementation() override
	{
		bool Result = false;
		for (auto& p : Party)
		{
			if (p.PartyCharacterData.StatusEffects.Contains(EStatusEffect::POISON) && !(p.PartyCharacterData.StatusEffects.Contains(EStatusEffect::KO)))
			{
				float PoisonDamage = FMath::Floor(p.PartyCharacterData.MaxHP * 0.02f);
				p.PartyCharacterData.HP = FMath::Max(1.f, p.PartyCharacterData.HP - PoisonDamage );
				Result = true;
			}
		}

		if (Result)
		{
			OnWorldDamage.Broadcast(Result);
		}
	}
};

