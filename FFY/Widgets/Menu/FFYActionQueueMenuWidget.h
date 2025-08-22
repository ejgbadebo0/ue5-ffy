// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Widgets/FFYMenuWidget.h"
#include "FFYActionQueueMenuWidget.generated.h"

class UFFYActionQueueOptionWidget;
class AFFYBattleCharacter;
class AFFYAction;
/**
 * 
 */
UCLASS()
class FFY_API UFFYActionQueueMenuWidget : public UFFYMenuWidget
{
	GENERATED_BODY()


protected:
	
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TArray<UFFYActionQueueOptionWidget*> QueueOptions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<UFFYActionQueueOptionWidget> QueueOptionsClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 NumQueued = 0;

public:

	UFUNCTION(BlueprintCallable, Category = "References")
	void InitializeParty(TArray<AFFYBattleCharacter*> Party);

	UFUNCTION()
	void AddToQueue(AFFYAction* Action, AFFYBattleCharacter* User, TArray<AFFYBattleCharacter*> ActionTargets);

	UFUNCTION(BlueprintImplementableEvent)
	void OnQueue();

	UFUNCTION()
	void OnDequeue();

	UFUNCTION(BlueprintImplementableEvent)
	void OnEmpty();
};
