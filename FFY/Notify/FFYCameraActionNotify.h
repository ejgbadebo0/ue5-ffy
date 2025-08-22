// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "FFY/FFYBattleEvents.h"
#include "FFY/FFYDataStructures.h"
#include "FFYCameraActionNotify.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API UFFYCameraActionNotify : public UAnimNotify
{
	GENERATED_BODY()

	public:
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Notify", meta=(AllowPrivateAccess=true))
	TArray<FCameraActionContainer> CameraActionContainers;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                    const FAnimNotifyEventReference& EventReference) override
	{
		Super::Notify(MeshComp, Animation, EventReference);

		IFFYBattleEvents* Character = Cast<IFFYBattleEvents>(MeshComp->GetOwner());
		if (Character && CameraActionContainers.Num() > 0)
		{
			int RandomIndex = FMath::RandRange(0, CameraActionContainers.Num() - 1);
			float Rand = FMath::FRandRange(0.f, 1.f);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%f"), Rand));
			if (Rand < CameraActionContainers[RandomIndex].Probability)
			{
				Character->StartCameraAction_Implementation(CameraActionContainers[RandomIndex]);
			}
		}
	}
};
 