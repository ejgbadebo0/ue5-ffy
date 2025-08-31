// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyState_TimedNiagaraEffect.h"
#include "FFY/FFYBattleEvents.h"
#include "FFYElementalNiagaraEffectState.generated.h"

enum class EElement : uint8;
class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class FFY_API UFFYElementalNiagaraEffectState : public UAnimNotifyState_TimedNiagaraEffect
{
	GENERATED_BODY()

public:


	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Anim Notify")
	TMap<EElement, TObjectPtr<UNiagaraSystem>> ElementTemplates;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override
	{
		IFFYBattleEvents* Character = Cast<IFFYBattleEvents>(MeshComp->GetOwner());
		if (Character)
		{
			FDamageAttributes DamageAttributes = Character->GetDamageAttributes_Implementation();
			TObjectPtr<UNiagaraSystem> Niagara = *ElementTemplates.Find(DamageAttributes.DamageElement);
			if (Niagara)
			{
				Template = Niagara;
			}
		}
		
		Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	}

};
