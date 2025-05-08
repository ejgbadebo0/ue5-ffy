// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYPlayerController.h"

#include "FFYTransitions.h"

void AFFYPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//OnPossessedPawnChanged.AddUniqueDynamic(this, &AFFYPlayerController::CallOnPossessed);
	
}

void AFFYPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//call custom interface function
	CallOnPossessed(InPawn);
}

void AFFYPlayerController::CallOnPossessed(APawn* NewPawn)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("CALLONPOSSESS"));
	if (NewPawn != nullptr)
	{
		IFFYTransitions* CallTarget = Cast<IFFYTransitions>(NewPawn);
		if (CallTarget)
		{
			CallTarget->Execute_OnPossessed(NewPawn);
		}
	}
}
