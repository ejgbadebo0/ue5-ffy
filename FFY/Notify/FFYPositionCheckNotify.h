// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "FFY/FFYBattleEvents.h"
#include "FFYPositionCheckNotify.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API UFFYPositionCheckNotify : public UAnimNotify
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
			if (Character->PositionCheck_Implementation(Distance))
			{
				MeshComp->GetAnimInstance()->StopAllMontages(0.2f);
			};
		}
	}
};
