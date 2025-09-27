// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "CoreMinimal.h"
#include "FFYDataEnums.h"

#include "FFYItemDataStructs.generated.h"

enum class EStatusEffect : uint8;
enum class EElement : uint8;
enum class EAttackType : uint8;
enum class EDamageModifier : uint8;
/**
 * 
 */
class FFY_API FFYItemDataStructs
{
public:
	FFYItemDataStructs();
	~FFYItemDataStructs();


};

UENUM(BlueprintType)
enum class EItemFuncCode : uint8
{
	NONE UMETA(DisplayName = "None"),
	POTION UMETA(DisplayName = "Potion"),
	ETHER UMETA(DisplayName = "Ether"),
	ELIXIR UMETA(DisplayName = "Elixir"),
	PHOENIX UMETA(DisplayName = "Phoenix"),
	ANTIDOTE UMETA(DisplayName = "Antidote"),
	E_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	NONE UMETA(DisplayName = "None"),
	WEAPON UMETA(DisplayName = "Weapon"),
	SHIELD UMETA(DisplayName = "Shield"),
	ARMOR UMETA(DisplayName = "Armor"),
	ACCESSORY UMETA(DisplayName = "Accessory"),
	E_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EEquipmentClass : uint8
{
	NONE UMETA(DisplayName = "None"),
	WARRIOR UMETA(DisplayName = "Warrior"),
	BMAGE UMETA(DisplayName = "Black Mage"),
	WMAGE UMETA(DisplayName = "White Mage"),
	THIEF UMETA(DisplayName = "Thief"),
	E_MAX UMETA(Hidden)
};


USTRUCT(BlueprintType)
struct FFY_API FEquipmentData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEquipmentSlot EquipmentSlot = EEquipmentSlot::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEquipmentClass EquipmentClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageModifier DamageModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttackType AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EElement Element;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStatusEffect StatusEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HP = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MP = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Strength = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Constitution = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Magic = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Spirit = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Precision = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Dexterity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Luck = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EElement> Weaknesses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EElement> Resistances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EElement> Catalysts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EStatusEffect> Immunities;
	
};


USTRUCT(BlueprintType)
struct FFY_API FItemData : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description = FText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Amount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FEquipmentData EquipmentData = FEquipmentData();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemFuncCode ItemFuncCode = EItemFuncCode::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETargetType TargetType = ETargetType::SINGLE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsConsumable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsEquipable = false;
};

