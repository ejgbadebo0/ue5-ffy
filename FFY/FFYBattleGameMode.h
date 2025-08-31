// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFYBattleEvents.h"

#include "GameFramework/GameModeBase.h"
#include "FFYBattleGameMode.generated.h"


class AFFYBattleCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemiesLoaded, TArray<AFFYBattleCharacter*>, Enemies);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBattleStarted);

USTRUCT(BlueprintType)
struct FFY_API FEnemyEncounters 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AFFYBattleCharacter>> EnemyClasses;
};

USTRUCT(BlueprintType)
struct FFY_API FMapEnemies : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEnemyEncounters> EnemyEncounters;
};

/**
 * 
 */
UCLASS()
class FFY_API AFFYBattleGameMode : public AGameModeBase, public IFFYBattleEvents
{
	GENERATED_BODY()
	//generate the list of enemies per map and give to the pawn to spawn

public:

	//DELEGATES:
	//UPROPERTY(BlueprintAssignable)
	FOnEnemiesLoaded OnEnemiesLoaded;
	FOnBattleStarted OnBattleStarted;
	//----------

	UPROPERTY(EditAnywhere,  BlueprintReadOnly, meta = (RowType = "MapEnemies"))
	FDataTableRowHandle BattleMapTableHandle;
	
	virtual void StartPlay() override;

	virtual void StartBattle_Implementation() override
	{
		OnBattleStarted.Broadcast();
	}
};
