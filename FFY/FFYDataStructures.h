// Source code implementation by Ephraim Gbadebo.

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


USTRUCT(BlueprintType)
struct FFY_API FBattleEXPData
{

	GENERATED_BODY()

	//used for display in menus
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName CharacterName = NAME_None;

	//total exp rewarded from battle
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TotalEXPGain = 0.f;
	
	//levels gained on character after applying total exp
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int LevelsGained = 0;

	//character lv before reward
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int InitialLV = 0;

	//exp value of character before reward
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialEXPValue = 0.f;

	//req exp value of character before reward
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialReqEXPValue = 0.f;

	//final EXP value to set on character
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EXPValueResult = 0.f;

	//final ReqEXP value to set on character
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReqEXPValueResult = 0.f;
};

USTRUCT(BlueprintType)
struct FFY_API FCameraActionData
{
	GENERATED_BODY()

	//Total duration allowed for camera cut
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 0.f;

	//duration that timeline has to lerp to target
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimelineDuration = 1.f;
	
	//how fast camera will lerp transforms
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LerpSpeed = 1.f;

	//if none, will use world origin for relative location and custom rotation variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECameraTargetType StartCameraTargetType = ECameraTargetType::NONE;

	//if none, will use world origin for relative location and custom rotation variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECameraTargetType EndCameraTargetType = ECameraTargetType::NONE;

	//if focusing on specific bone socket on target
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName = NAME_None;

	//relative to target denoted by StartCameraTargetType, if 0 will use current camera transform
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector RelativeStartLocation = FVector::ZeroVector;

	//relative to target denoted by EndCameraTargetType, if 0 will use current camera transform
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector RelativeEndLocation = FVector::ZeroVector;

	//used when CameraTargetType is none
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator CustomStartRotation = FRotator::ZeroRotator;

	//used when CameraTargetType is none
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator CustomEndRotation = FRotator::ZeroRotator;

	//if 0, will not affect camera zoom
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartZoomOffset = 0.f;

	//if 0, will not affect camera zoom
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EndZoomOffset = 0.f;
};

USTRUCT(BlueprintType)
struct FFY_API FCameraActionContainer
{
	GENERATED_BODY()

	//current CameraActions will be overriden by others with higher values
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Priority = 0;

	//if true, can override CameraActions of the same level of priority
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanOverride = false;

	// 0-1: if random float exceeds value, this CameraAction won't be played
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Probability = 1.f;

	//set of data structs for a complete CameraAction
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCameraActionData> CameraActions;
};