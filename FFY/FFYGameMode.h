// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FFYCameraControls.h"
#include "GameFramework/GameModeBase.h"
#include "FFYGameMode.generated.h"

class AFFYCharacter;
class UFFYGameInstance;

UCLASS(minimalapi)
class AFFYGameMode : public AGameModeBase, public IFFYCameraControls
{
	GENERATED_BODY()

public:
	AFFYGameMode();

	UFUNCTION(BlueprintCallable, Category="AFFY")
	void InitSceneCameras();

protected:
	virtual void BeginPlay() override;

public:
	virtual void StartPlay() override;

	virtual void SetActiveViewTarget(AActor* Camera) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="References")
	AFFYCharacter* CharacterReference;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="References")
	UFFYGameInstance* GameInstance;
};



