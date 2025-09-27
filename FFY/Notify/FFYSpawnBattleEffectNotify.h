// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "KeyParams.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "FFY/FFYActionContainer.h"
#include "FFY/FFYBattleCharacter.h"
#include "FFY/FFYBattleEffect.h"
#include "Kismet/GameplayStatics.h"
#include "FFYSpawnBattleEffectNotify.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API UFFYSpawnBattleEffectNotify : public UAnimNotify
{
	GENERATED_BODY()

public:


	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Notify", meta=(AllowPrivateAccess=true))
	TSubclassOf<AFFYBattleEffect> SpawnBattleEffectClass;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Notify", meta=(AllowPrivateAccess=true))
	bool bUseSocket;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

	virtual bool ShouldFireInEditor() override
	{
		return false;
	}
};

inline void UFFYSpawnBattleEffectNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	Super::Notify(MeshComp, Animation, EventReference);

	IFFYBattleEvents* User = Cast<IFFYBattleEvents>(MeshComp->GetOwner());
	if (User)
	{
		User->BeginSpawnBattleEffect_Implementation(SpawnBattleEffectClass, bUseSocket);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 35.f, FColor::White, "Failed to cast battle effect");
	}
	
}
