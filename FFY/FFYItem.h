// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FFYBattleCharacter.h"
#include "FFYDataStructures.h"
#include "UObject/NoExportTypes.h"
#include "FFYItemDataStructs.h"
#include "FFYItem.generated.h"

USTRUCT(BlueprintType)
struct FFY_API FItemUseResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bWasUsed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amount = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ResultDescription = FText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStat StatChanged = EStat::HP;
};


/**
 * Manager class that contains functions for usable items. 
 */
UCLASS(Blueprintable)
class FFY_API UFFYItem : public UObject
{
	GENERATED_BODY()

	
public:

	void DecrementItem(FItemData& Item);

	void ModifyStats(FItemData& Item, FBattleCharacterData& Target, float Value = 1.f);
	
	virtual void Use(FItemData& Item, FBattleCharacterData& Target);

	virtual void BattleUse(FItemData& Item, AFFYBattleCharacter* User, AFFYBattleCharacter* Target);

	FItemUseResult ItemEffect(EItemFuncCode ItemFuncCode, FBattleCharacterData& Target);

	virtual void Equip(FItemData& Item, FBattleCharacterData& Target);

	virtual void Unequip(FItemData& Item, FBattleCharacterData& Target); //nf
	
	virtual void Unequip(EEquipmentSlot Slot, FBattleCharacterData& Target);

	virtual FEquipmentData Compare(FItemData& Item, FBattleCharacterData& Target);
};
