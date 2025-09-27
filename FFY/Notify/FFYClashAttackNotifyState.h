// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "FFY/FFYBattleEvents.h"
#include "FFYClashAttackNotifyState.generated.h"

/**
 * An attack that has a duration in which it can clash with another attack.
 */
UCLASS()
class FFY_API UFFYClashAttackNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int HitIndex = 0;
	
	virtual FLinearColor GetEditorColor() override
	{
		return FLinearColor(1.f, 0.8f, 0.8f);
	}

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
							 const FAnimNotifyEventReference& EventReference) override
	{
		Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

		IFFYBattleEvents* Character = Cast<IFFYBattleEvents>(MeshComp->GetOwner());
		if (Character)
		{
			Character->SetCanClash_Implementation(true);	
		}
	}

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                       const FAnimNotifyEventReference& EventReference) override
	{
		Super::NotifyEnd(MeshComp, Animation, EventReference);

		IFFYBattleEvents* Character = Cast<IFFYBattleEvents>(MeshComp->GetOwner());
		if (Character)
		{
			Character->TriggerBattleEffect_Implementation(HitIndex);
			Character->SetCanClash_Implementation(false);	
		}
	}
	
};
