// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FFY/FFYBattleEvents.h"
#include "FFYChainWidget.generated.h"


UCLASS()
class FFY_API UFFYChainWidget : public UUserWidget, public IFFYBattleEvents
{
	
	GENERATED_BODY()
	
public:
	virtual void AddFallOff_Implementation(float FallOff) override
	{
		OnAddFallOff(FallOff);
	}

	virtual void ResetChain_Implementation() override
	{
		OnResetChain();
	}

	virtual void UpdateChain_Implementation(int Amount, float FallOffRate) override
	{
		OnUpdateChain(Amount, FallOffRate);
	}

	virtual void UpdateFallOff_Implementation(float Value) override
	{
		OnUpdateFallOff(Value);
	}

	//BP REP EVENTS:

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateChain(int Amount, float FallOffRate);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateFallOff(float Value);

	UFUNCTION(BlueprintImplementableEvent)
	void OnAddFallOff(float FallOff);

	UFUNCTION(BlueprintImplementableEvent)
	void OnResetChain();
};
