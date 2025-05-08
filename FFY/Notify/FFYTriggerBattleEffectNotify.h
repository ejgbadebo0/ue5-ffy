// Fill out your copyright notice in the Description page of Project Settings.

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

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                    const FAnimNotifyEventReference& EventReference) override
	{
		Super::Notify(MeshComp, Animation, EventReference);

		IFFYBattleEvents* User = Cast<IFFYBattleEvents>(MeshComp->GetOwner());
		if (User)
		{
			User->TriggerBattleEffect_Implementation(); 
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 35.f, FColor::White, "Failed to cast battle effect");
		}
	}

	virtual bool ShouldFireInEditor() override
	{
		return false;
	}
	
};
