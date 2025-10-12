// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "FFY/FFYBattleEvents.h"
#include "FFYTriggerBattleEffectNotify.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API UFFYTriggerBattleEffectNotify : public UAnimNotify
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Notify", meta=(AllowPrivateAccess=true))
	int HitIndex = 0;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                    const FAnimNotifyEventReference& EventReference) override
	{
		Super::Notify(MeshComp, Animation, EventReference);

		IFFYBattleEvents* User = Cast<IFFYBattleEvents>(MeshComp->GetOwner());
		if (User)
		{
			User->TriggerBattleEffect_Implementation(HitIndex); 
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 35.f, FColor::White, "Failed to cast battle effect");
		}
	}
	
};
