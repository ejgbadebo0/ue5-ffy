// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CoreMinimal.h"

#include "FFYDataEnums.generated.h"

/**
 * 
 */
class FFY_API FFYDataEnums
{
public:
	FFYDataEnums();
	~FFYDataEnums();
};

UENUM(BlueprintType)
enum class EMenuMode : uint8
{
	NONE UMETA(DisplayName = "None"),
	SELECTING UMETA(DisplayName = "Selecting"),
	E_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ETargetType : uint8
{
	NONE UMETA(DisplayName = "None"),
	SINGLE UMETA(DisplayName = "Single"),
	MULTI UMETA(DisplayName = "Multi"),
	BOTH UMETA(DisplayName = "Both"),
	E_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EStatusEffect : uint8
{
	//Negative
	NONE UMETA(DisplayName = "None"),
	POISON UMETA(DisplayName = "Poison"),
	BLIND UMETA(DisplayName = "Blind"),
	SILENCE UMETA(DisplayName = "Silence"),
	PARALYSIS UMETA(DisplayName = "Paralysis"),
	SLEEP UMETA(DisplayName = "Sleep"),
	CONFUSE UMETA(DisplayName = "Confuse"),
	BERSERK UMETA(DisplayName = "Berserk"),
	PETRIFY UMETA(DisplayName = "Petrify"),
	DOOM UMETA(DisplayName = "Doom"),
	DEATH UMETA(DisplayName = "Death"),
	SLOW UMETA(DisplayName = "Slow"),
	STOP UMETA(DisplayName = "Stop"),
	MINI UMETA(DisplayName = "Mini"),
	TOAD UMETA(DisplayName = "Toad"),
	ZOMBIE UMETA(DisplayName = "Zombie"),
	BURN UMETA(DisplayName = "Burn"),
	FREEZE UMETA(DisplayName = "Freeze"),
	SHOCK UMETA(DisplayName = "Shock"),
	DRENCH UMETA(DisplayName = "Drench"),
	STAGGER UMETA(DisplayName = "Stagger"),
	KO UMETA(DisplayName = "KO"),
	//Positive
	E_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EElement : uint8
{
	NONE UMETA(DisplayName = "None"),
	FIRE UMETA(DisplayName = "Fire"),
	ICE UMETA(DisplayName = "Ice"),
	THUNDER UMETA(DisplayName = "Thunder"),
	WATER UMETA(DisplayName = "Water"),
	WIND UMETA(DisplayName = "Wind"),
	EARTH UMETA(DisplayName = "Earth"),
	LIGHT UMETA(DisplayName = "Light"),
	DARKNESS UMETA(DisplayName = "Darkness"),
	COSMIC UMETA(DisplayName = "Cosmic"),
	HEALING UMETA(DisplayName = "Healing"),
	STATUS UMETA(DisplayName = "Status"),
	E_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EDamageModifier : uint8
{
	NONE UMETA(DisplayName = "None"),
	PHYSICAL UMETA(DisplayName = "Physical"),
	MAGIC UMETA(DisplayName = "Magic"),
	E_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	NONE UMETA(DisplayName = "None"),
	MELEE UMETA(DisplayName = "Melee"),
	RANGED UMETA(DisplayName = "Ranged"),
	E_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EStat : uint8
{
	NONE UMETA(DisplayName = "None"),
	MP UMETA(DisplayName = "MP"),
	HP UMETA(DisplayName = "HP"),
	LP UMETA(DisplayName = "LP"),
	E_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	NONE UMETA(DisplayName = "None"),
	IDLE UMETA(DisplayName = "Idle"),
	ACTING UMETA(DisplayName = "Acting "),
	CASTING UMETA(DisplayName = "Casting"),
	DEFENDING UMETA(DisplayName = "Defending"),
	E_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EActiveState : uint8
{
	ACTIVE UMETA(DisplayName = "Active"),
	WAIT UMETA(DisplayName = "Wait"),
	EMAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ECameraTargetType : uint8
{
	NONE UMETA(DisplayName = "None"),
	USER UMETA(DisplayName = "User"),
	TARGET UMETA(DisplayName = "Target"),
	EFFECT UMETA(DisplayName = "Effect"),
	E_MAX UMETA(Hidden)
};
