// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "FFY/FFYBattleEvents.h"
#include "FFYContextCommandNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API UFFYContextCommandNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:


	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	FName ContextCommandName;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override
	{
		Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

		IFFYBattleEvents* Character = Cast<IFFYBattleEvents>(MeshComp->GetOwner());
		if (Character)
		{
			Character->EnableContextCommand_Implementation(ContextCommandName);
		}
	}

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                       const FAnimNotifyEventReference& EventReference) override
	{
		Super::NotifyEnd(MeshComp, Animation, EventReference);
		IFFYBattleEvents* Character = Cast<IFFYBattleEvents>(MeshComp->GetOwner());
		if (Character)
		{
			Character->DisableContextCommand_Implementation();
		}
	}
};
