// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "FFY/FFYBattleEvents.h"
#include "FFYActionStateNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API UFFYActionStateNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	EActionState ActionState = EActionState::ACTING;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool bSetWait = false;

	virtual FLinearColor GetEditorColor() override
	{
		return FLinearColor(0.274509f, 0.5568627f, 0.0f);
	}

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override
	{
		Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

		IFFYBattleEvents* Character = Cast<IFFYBattleEvents>(MeshComp->GetOwner());
		if (Character)
		{
			Character->SetActionState_Implementation(ActionState, bSetWait);
		}
	}

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
						   const FAnimNotifyEventReference& EventReference) override
	{
		Super::NotifyEnd(MeshComp, Animation, EventReference);

		IFFYBattleEvents* Character = Cast<IFFYBattleEvents>(MeshComp->GetOwner());
		if (Character)
		{
			Character->SetActionState_Implementation(EActionState::IDLE, false);
		}
	}
};
