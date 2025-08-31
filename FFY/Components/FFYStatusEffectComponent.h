// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "FFY/Widgets/Menu/FFYStatusMenuWidget.h"
#include "FFYStatusEffectComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKOEffectFinished);

/**
 *  Used to manage visual effects of Status Effects inflicted.
 */
UCLASS()
class FFY_API UFFYStatusEffectComponent : public UWidgetComponent
{
	GENERATED_BODY()


public:
	//DELEGATES:
	FOnKOEffectFinished OnKOEffectFinished;
	//========

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AddEffect(EStatusEffect Effect);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void RemoveEffect(EStatusEffect Effect);

	UFUNCTION(BlueprintCallable)
	void CallKOEffectFinished()
	{
		OnKOEffectFinished.Broadcast();
	}
};
