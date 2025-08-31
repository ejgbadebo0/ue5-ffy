// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FFYBattleSpawnContainer.generated.h"

class AFFYBattleSpawnTransform;

UCLASS()
class FFY_API AFFYBattleSpawnContainer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFFYBattleSpawnContainer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AFFYBattleSpawnTransform*> PartySpawns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AFFYBattleSpawnTransform*> EnemySpawns;

};
