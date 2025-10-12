// Source code implementation by Ephraim Gbadebo.


#include "FFYBattleGameMode.h"

#include "FFYBattleCharacter.h"
#include "FFYBattleSpawnContainer.h"
#include "FFYBattleSpawnTransform.h"
#include "FFYPlayerBattleManager.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
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
				FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
				SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				AFFYBattleCharacter* EnemySpawn = GetWorld()->SpawnActor<AFFYBattleCharacter>(EnemyEncounter.EnemyClasses[i], SpawnContainer->EnemySpawns[i]->GetActorTransform(), SpawnParameters);
				LoadedEnemies.Emplace(EnemySpawn);
			}

			OnEnemiesLoaded.Broadcast(LoadedEnemies);
		}
	}

	IFFYMusicEvents* GameInstance = Cast<IFFYMusicEvents>(GetWorld()->GetGameInstance());
	if (GameInstance)
	{
		FMusicData MusicData = GetDefaultMusicData_Implementation();
		GameInstance->PlayMusic_Implementation(MusicData);
	}
}

void AFFYBattleGameMode::StartBattle_Implementation()
{
	FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
	FName ID = FName(*LevelName);

	FBattleMapSequence* MapSequences = BattleMapSequenceTableHandle.DataTable->FindRow<FBattleMapSequence>(ID, "", true);
	if (MapSequences)
	{
		int Index = FMath::RandRange(0,MapSequences->LevelSequences.Num()-1);
		TSoftObjectPtr<ULevelSequence> Sequence = MapSequences->LevelSequences[Index];
		UWorld* World = GetWorld();
		
		//APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		FStreamableManager StreamableManager = FStreamableManager();
		StreamableManager.RequestAsyncLoad(Sequence.ToSoftObjectPath(), [this, Sequence, World]() 
		{
			UObject* Object = Sequence.Get();
			ULevelSequence* LevelSequence = Cast<ULevelSequence>(Object);
			if (LevelSequence)
			{
				ALevelSequenceActor* Actor;
				ULevelSequencePlayer* Player = ULevelSequencePlayer::CreateLevelSequencePlayer(World, LevelSequence, FMovieSceneSequencePlaybackSettings(), Actor);
				if (Player && Actor)
				{
					Player->RewindForReplay();
					Player->Play();
					Player->OnFinished.AddUniqueDynamic(this, &AFFYBattleGameMode::FinishStartBattle);
				}
				else
				{
					//couldn't find/assign player/actor
					FinishStartBattle();
				}
			}
			else
			{
				//LevelSequence not valid
				FinishStartBattle();
			}
		}); 
	}
	else
	{
		//couldn't find data table entry
		FinishStartBattle();
	}
}

void AFFYBattleGameMode::FinishStartBattle()
{
	OnBattleStarted.Broadcast();
}

FMusicData AFFYBattleGameMode::GetDefaultMusicData_Implementation()
{
	FMusicData MusicDataResult = FMusicData();
	if (!(MapMusicTableHandle.IsNull()))
	{
		FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
		FName ID = FName(*LevelName);
		
		FMusicData* MapMusicData = MapMusicTableHandle.DataTable->FindRow<FMusicData>(ID, "", true);
		if (MapMusicData != nullptr)
		{
			MusicDataResult.MusicTrack = MapMusicData->MusicTrack;
			MusicDataResult.LoopStartTime = MapMusicData->LoopStartTime;
			MusicDataResult.LoopEndPercent = MapMusicData->LoopEndPercent;
		}
		else
		{
			MapMusicData = MapMusicTableHandle.DataTable->FindRow<FMusicData>(FName("BattleDefault"), "", true);
			if (MapMusicData != nullptr)
			{
				MusicDataResult.MusicTrack = MapMusicData->MusicTrack;
				MusicDataResult.LoopStartTime = MapMusicData->LoopStartTime;
				MusicDataResult.LoopEndPercent = MapMusicData->LoopEndPercent;
			}
		}
	}

	return MusicDataResult;
}
