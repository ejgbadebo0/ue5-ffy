// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FFYPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API AFFYPlayerController : public APlayerController
{
	GENERATED_BODY()
	//update to include scene capture component

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	virtual void CallOnPossessed(APawn* NewPawn);
};
