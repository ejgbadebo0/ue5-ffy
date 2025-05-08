// Copyright Epic Games, Inc. All Rights Reserved.

#include "FFYGameMode.h"
#include "FFYCharacter.h"
#include "FFYGameInstance.h"
#include "FFYSceneCamera.h"
#include "UObject/ConstructorHelpers.h"

AFFYGameMode::AFFYGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AFFYGameMode::InitSceneCameras()
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, TEXT("Get GameInstance"));
	if (GameInstance != NULL)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, TEXT("Get Scene Cameras"));
		int32 SceneIndex = GameInstance->PlayerPreBattleInfo.LastSceneIndex;
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFFYSceneCamera::StaticClass(), OutActors);
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, FString::Printf(TEXT("%d actors"), OutActors.Num()) );
		if (OutActors[SceneIndex] != nullptr)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, TEXT("Get Player Actor"));
			
			if (CharacterReference != nullptr)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, TEXT("Call ViewTarget Func"));
				CharacterReference->SetActiveViewTarget(OutActors[SceneIndex]);
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, TEXT("INVALID INDEX"));
		}

		
		/*
		for (int i = 0; i < OutActors.Num(); i++)
		{
			if (OutActors[i] != nullptr)
			{
				FString Astr = OutActors[i]->GetName();
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, FString::Printf(TEXT("%s: %d"), *Astr, i));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, TEXT("INVALID INDEX"));
			}
		}
		*/
	}
}

void AFFYGameMode::BeginPlay()
{
	Super::BeginPlay();
	//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Black, TEXT("BeginPlay"));
	GameInstance = Cast<UFFYGameInstance>(GetGameInstance());
	AFFYCharacter* PlayerActor = Cast<AFFYCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerActor != nullptr)
	{
		CharacterReference = PlayerActor;
		InitSceneCameras();
	}
	
}

void AFFYGameMode::StartPlay()
{
	Super::StartPlay();
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Black, TEXT("StartPlay"));
	if (GameInstance != nullptr)
	{
		GameInstance->InitializeBattleMap();
	}
}

void AFFYGameMode::SetActiveViewTarget(AActor* Camera)
{
	if (GameInstance != NULL)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, TEXT("Get Scene Cameras"));
		int32 SceneIndex = GameInstance->PlayerPreBattleInfo.LastSceneIndex;
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFFYSceneCamera::StaticClass(), OutActors);
		if ( OutActors.Contains(Camera) )
		{
			int32 NewIndex = OutActors.IndexOfByKey(Camera);
			if (NewIndex != INDEX_NONE)
			{
				GameInstance->PlayerPreBattleInfo.LastSceneIndex = NewIndex;
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, TEXT("LastSceneIdx updated!"));
			}
		}
	}
}
