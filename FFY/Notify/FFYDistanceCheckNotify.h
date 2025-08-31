// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "FFY/FFYBattleEvents.h"
#include "FFYDistanceCheckNotify.generated.h"

/**
 * Check if target actor is within range
 */
UCLASS()
class FFY_API UFFYDistanceCheckNotify : public UAnimNotify
{
	GENERATED_BODY()

public:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Notify", meta=(AllowPrivateAccess=true))
	float Distance = 200.f;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
					const FAnimNotifyEventReference& EventReference) override
	{
		Super::Notify(MeshComp, Animation, EventReference);

		IFFYBattleEvents* Character = Cast<IFFYBattleEvents>(MeshComp->GetOwner());
		if (Character)
		{
			Character->DistanceCheck_Implementation(Distance);
		}
	}
};
