// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/FFYDataEnums.h"
#include "FFY/Widgets/Option/FFYContextCommandOptionWidget.h"
#include "FFYActionQueueOptionWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDequeue);

class AFFYAction;
/**
 * 
 */
UCLASS()
class FFY_API UFFYActionQueueOptionWidget : public UFFYContextCommandOptionWidget
{
	GENERATED_BODY()

public:

	FOnDequeue OnDequeue;
	
	UFUNCTION()
	void ExecuteAction(float Value);

	UFUNCTION()
	void ExecuteActionFromState(EActionState NewState);

	UFUNCTION()
	void ExecuteActionFromWait(EActiveState NewActiveState);
	
	virtual void Queue(AFFYAction* Action, AFFYBattleCharacter* Character, TArray<AFFYBattleCharacter*> Targets);
	virtual void Unload() override;
	virtual void PerformAction() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References", meta = (AllowPrivateAccess = "true"))
	AFFYAction* QueueAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References", meta = (AllowPrivateAccess = "true"))
	TArray<AFFYBattleCharacter*> QueueTargets;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References", meta = (AllowPrivateAccess = "true"))
	float TargetATB;
};
