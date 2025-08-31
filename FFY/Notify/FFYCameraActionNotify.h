// Source code implementation by Ephraim Gbadebo.

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

	virtual FLinearColor GetEditorColor() override
	{
		return FLinearColor(0.1625490f, 0.0f, 1.0f);
	}

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
 