// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FFYCameraControls.h"
#include "FFYDataStructures.h"
#include "FFYMusicEvents.h"
#include "GameFramework/GameModeBase.h"
#include "FFYGameMode.generated.h"

class AFFYCharacter;
class UFFYGameInstance;

UCLASS(minimalapi)
class AFFYGameMode : public AGameModeBase, public IFFYCameraControls, public IFFYMusicEvents
{
	GENERATED_BODY()

public:
	AFFYGameMode();

	UFUNCTION(BlueprintCallable, Category="AFFY")
	void InitSceneCameras();

	UFUNCTION()
	void InitPlayerTransform(bool Load, FTransform& InTransform);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (RowType = "MusicData"))
	FDataTableRowHandle MapMusicTableHandle;

protected:
	
	virtual void BeginPlay() override;

public:
	virtual void StartPlay() override;

	virtual void SetActiveViewTarget(AActor* Camera) override;

	virtual void SetActiveViewTargetBP_Implementation(AActor* Camera) override
	{
		SetActiveViewTarget(Camera);
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="References")
	AFFYCharacter* CharacterReference;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="References")
	UFFYGameInstance* GameInstance;


	virtual FMusicData GetDefaultMusicData_Implementation() override;

	virtual bool GetCameraControlRotationDefault_Implementation() override
	{
		return true;
	}
};



