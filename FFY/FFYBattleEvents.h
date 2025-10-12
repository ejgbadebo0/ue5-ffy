// Source code implementation by Ephraim Gbadebo.

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
	bool GetIsDead();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetActiveState(EActiveState NewState);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetActionState(EActionState NewState, bool bSetWait);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCanClash(bool Value);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnStartDefendAction(float ATBValue);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartBattle();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EndBattle();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DestroyNiagaraEffect(UNiagaraSystem* NiagaraComponent);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BeginSpawnBattleEffect(TSubclassOf<AFFYBattleEffect> BattleEffectClass, bool bUseSocket);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TriggerBattleEffect(int HitIndex);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartCameraAction(FCameraActionContainer CameraActionContainer);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EndIntroSequence(FTransform CurrentSequenceTransform); 

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UseInventoryItem(FName ID, AFFYBattleCharacter* Source, AFFYBattleCharacter* Target);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddFromDrop(FName ItemID, int32 Amount);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EnableContextCommand(FName ActionName, float Duration);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DisableContextCommand();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ActionUsed(FName ActionName, bool bIsEnemy);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FName GetItemName(FName ID);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool DistanceCheck(float Distance);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool PositionCheck(float Distance);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdatePartyMemberStats(FBattleCharacterData CharacterData);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int GetChain();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateChain(int Amount, float FallOffRate);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ResetChain();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddFallOff(float FallOff);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateFallOff(float Value);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EvaluateStatusEffects();

	

};
