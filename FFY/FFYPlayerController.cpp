// Source code implementation by Ephraim Gbadebo.


#include "FFYPlayerController.h"

#include "GameFramework/GameMode.h"
#include "FFYCameraControls.h"
#include "FFYTransitions.h"


void AFFYPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//IFFYCameraControls* GameMode = Cast<IFFYCameraControls>(GetWorld()->GetAuthGameMode());
	//bAutoManageActiveCameraTarget = (GameMode) ? GameMode->GetCameraControlRotationDefault_Implementation() : true;
	
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
