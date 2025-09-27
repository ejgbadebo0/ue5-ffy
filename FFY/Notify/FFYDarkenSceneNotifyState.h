// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "FFY/FFYCameraControls.h"
#include "Kismet/GameplayStatics.h"
#include "FFYDarkenSceneNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API UFFYDarkenSceneNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()


public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override
	{
		Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

		APawn* Pawn = UGameplayStatics::GetPlayerPawn(MeshComp->GetOwner()->GetWorld(), 0);
		if (Pawn == nullptr) { return; }
		IFFYCameraControls* Manager = Cast<IFFYCameraControls>(Pawn);
		//GEngine->AddOnScreenDebugMessage(-1, 555.f, FColor::Cyan, FString::Printf(TEXT("DARKEN: %s"), *Pawn->GetName()));
		if (Manager)
		{
			Manager->DarkenScene_Implementation(true);
		}
	}

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                       const FAnimNotifyEventReference& EventReference) override
	{
		Super::NotifyEnd(MeshComp, Animation, EventReference);

		APawn* Pawn = UGameplayStatics::GetPlayerPawn(MeshComp->GetOwner()->GetWorld(), 0);
		if (Pawn == nullptr) { return; }
		IFFYCameraControls* Manager = Cast<IFFYCameraControls>(Pawn);
		if (Manager)
		{
			Manager->DarkenScene_Implementation(false);
		}
	}
};
