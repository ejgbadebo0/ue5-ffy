// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FFYDataEnums.h"
//#include "FFYAction.h"
#include "CoreMinimal.h"
#include "FFYItemDataStructs.h"

#include "FFYDataStructures.generated.h"


class AFFYBattleCharacter;
/**
 * 
 */
class FFY_API FFYDataStructures
{
public:
	FFYDataStructures();
	~FFYDataStructures();

};

USTRUCT(BlueprintType)
struct FFY_API FDamageAttributes
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageAmount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStat DamageStat = EStat::HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageModifier DamageModifier = EDamageModifier::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EElement DamageElement = EElement::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EStatusEffect> Inflicts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EStatusEffect> Removes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttackType AttackType = EAttackType::NONE;
};

USTRUCT(BlueprintType)
struct FFY_API FEquipment {

	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemData Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemData Shield;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemData Armor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemData Accessory;

};

USTRUCT(BlueprintType)
struct FFY_API FBattleCharacterData : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AFFYBattleCharacter> BattleCharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEquipmentClass EquipmentClass = EEquipmentClass::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CharacterName = NAME_None; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LV = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxMP = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Strength = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Constitution = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Magic = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Spirit = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Precision = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Dexterity = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Luck = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EXP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ReqEXP = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EStatusEffect> StatusEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EElement> Weaknesses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EElement> Resistances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EElement> Catalysts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EStatusEffect> Immunities;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TArray<TSubclassOf<AFFYAction>> Actions;
	
	//equipment
	FEquipment Equipment;

	

	//abilities can be assigned through a higher level struct (maybe defined in GameInstance)
};