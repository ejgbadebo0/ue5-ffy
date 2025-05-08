// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FFYDataStructures.h"
#include "UObject/Interface.h"
#include "FFYBattleEvents.generated.h"

class AFFYBattleEffect;
class UNiagaraSystem;
class AFFYBattleCharacter;
class AFFYAction;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFFYBattleEvents : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FFY_API IFFYBattleEvents
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:

	//
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool ReceiveDamage(AFFYBattleCharacter* Source, AFFYAction* SourceAction);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool ReceiveHealing(AFFYBattleCharacter* Source, AFFYAction* SourceAction);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FDamageAttributes GetDamageAttributes();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnStartDefendAction(float ATBValue);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartBattle();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DestroyNiagaraEffect(UNiagaraSystem* NiagaraComponent);

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	//void (AFFYAction* Action, AFFYBattleCharacter* User, TArray<AFFYBattleCharacter*> Targets);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BeginSpawnBattleEffect(TSubclassOf<AFFYBattleEffect> BattleEffectClass);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TriggerBattleEffect();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UseInventoryItem(FName ID, AFFYBattleCharacter* Source, AFFYBattleCharacter* Target);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EnableContextCommand(FName ActionName);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DisableContextCommand();
};
