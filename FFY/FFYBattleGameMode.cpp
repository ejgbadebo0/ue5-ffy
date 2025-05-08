// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYBattleGameMode.h"

#include "FFYBattleCharacter.h"
#include "FFYBattleSpawnContainer.h"
#include "FFYBattleSpawnTransform.h"
#include "FFYPlayerBattleManager.h"
#include "Kismet/GameplayStatics.h"



void AFFYBattleGameMode::StartPlay()
{
	Super::StartPlay();
	//GEngine->AddOnScreenDebugMessage(-1, 35.f, FColor::Red, "StartPlay");
	FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
	FName ID = FName(*LevelName);

	FMapEnemies* MapEnemies = BattleMapTableHandle.DataTable->FindRow<FMapEnemies>(ID, "", true);

	if (MapEnemies)
	{
		int EncounterIndex = FMath::RandRange(0,MapEnemies->EnemyEncounters.Num()-1);
		AFFYBattleSpawnContainer* SpawnContainer = Cast<AFFYBattleSpawnContainer>(UGameplayStatics::GetActorOfClass(GetWorld(), AFFYBattleSpawnContainer::StaticClass()));
		
		FEnemyEncounters EnemyEncounter = MapEnemies->EnemyEncounters[EncounterIndex];
		if (SpawnContainer)
		{
			TArray<AFFYBattleCharacter*> LoadedEnemies;
			
			for (int i = 0; i < EnemyEncounter.EnemyClasses.Num() && i < SpawnContainer->EnemySpawns.Num(); i++)
			{
				AFFYBattleCharacter* EnemySpawn = GetWorld()->SpawnActor<AFFYBattleCharacter>(EnemyEncounter.EnemyClasses[i], SpawnContainer->EnemySpawns[i]->GetActorTransform());
				LoadedEnemies.Emplace(EnemySpawn);
			}

			OnEnemiesLoaded.Broadcast(LoadedEnemies);
		}
	}
}