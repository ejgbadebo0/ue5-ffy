// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//ext includes
#include "FFYDataStructures.h"
#include "MathUtil.h"
//----
#include "FFYAction.generated.h"

class AFFYBattleCharacter;

UENUM(BlueprintType)
enum class EActionType : uint8
{
	NONE UMETA(DisplayName = "None"),
	MENU UMETA(DisplayName = "Menu"),
	MAGIC UMETA(DisplayName = "Magic"),
	SKILL UMETA(DisplayName = "Skill"),
	ITEM UMETA(DisplayName = "Item"),
	STATE UMETA(DisplayName = "State"),
	LIMIT UMETA(DisplayName = "Limit"),
	E_MAX UMETA(Hidden)
};

UCLASS(Abstract)
class FFY_API AFFYAction : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	//AFFYAction();

	bool operator==(AFFYAction* action) const
	{
		return this->Label == action->Label;
	}

	/* Unique action name */
	UPROPERTY(EditDefaultsOnly)
	FName Label = "None";

	//for use in specific menus
	UPROPERTY(EditDefaultsOnly)
	FName MenuLabel = "None";

	/* information that is displayed in menus */
	UPROPERTY(EditDefaultsOnly)
	FText Description = FText::GetEmpty();

	/* 0 - CombatOnly 1 - Magic/Combat&Menu 2 - MenuOnly */
	UPROPERTY(EditDefaultsOnly)
	uint8 Type = 0;

	/* how many can be targeted by this action */
	UPROPERTY(EditDefaultsOnly)
	ETargetType TargetType = ETargetType::NONE;
	
	
	/* Type of action */
	UPROPERTY(EditDefaultsOnly)
	EActionType ActionType = EActionType::NONE;

	/* will target the other group by default */
	UPROPERTY(EditDefaultsOnly)
	bool bIsDefaultTargetEnemy = true;

	/* if false, action cannot be Queued */
	UPROPERTY(EditDefaultsOnly)
	bool bQueuedAction = false;

	/* effects happen at the start of battle and persist */
	UPROPERTY(EditDefaultsOnly)
	bool bIsPassive = false;

	/* How much ATB gauge taking the action will cost, Menu types will not cost any ATB by default, can be 0-100. */
	UPROPERTY(EditDefaultsOnly)
	float ATBCost = 100;

	UPROPERTY(EditDefaultsOnly)
	float HPCost = 0;

	UPROPERTY(EditDefaultsOnly)
	float MPCost = 0;

	UPROPERTY(EditDefaultsOnly)
	float LPCost = 0;

	UPROPERTY(EditDefaultsOnly)
	float Rank = 1;

	/*
	FName Label = "None";

	uint8 Type = 0;

	EActionType ActionType = EActionType::NONE;
	
	bool bQueuedAction = false;

	float ATBCost = 100;

	float HPCost = 0;

	float MPCost = 0;

	float LPCost = 0;
	 */

	/*
	UFUNCTION(BlueprintCallable)
	virtual FName GetLabel() const { return Label; }

	UFUNCTION(BlueprintCallable)
	virtual FText GetDescription() const { return Description; }

	UFUNCTION(BlueprintCallable)
	virtual uint8 GetType() const { return Type; }

	UFUNCTION(BlueprintCallable)
	virtual EActionType GetActionType() const { return ActionType; }

	UFUNCTION(BlueprintCallable)
	virtual ETargetType GetTargetType() const { return TargetType; }

	UFUNCTION(BlueprintCallable)
	virtual bool GetIsQueuedAction() const { return bQueuedAction; }

	UFUNCTION(BlueprintCallable)
	virtual float GetATBCost() const { return ATBCost; }

	UFUNCTION(BlueprintCallable)
	virtual float GetHPCost() const { return HPCost; }

	UFUNCTION(BlueprintCallable)
	virtual float GetMPCost() const { return MPCost; }

	UFUNCTION(BlueprintCallable)
	virtual float GetLPCost() const { return LPCost; }

	UFUNCTION(BlueprintCallable)
	virtual float GetRank() const { return Rank; }
	*/
	
	UFUNCTION(BlueprintCallable)
	virtual float GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner);

	UFUNCTION(BlueprintCallable)
	virtual float GetVariableATBCost(AFFYBattleCharacter* ActionOwner);

	UFUNCTION(BlueprintCallable)
	virtual FName GetMenuLabel();
	
	/*
	 * Attempt to use action in battle.
	*/
	UFUNCTION(BlueprintCallable)
	virtual void ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets);

	/*
	 * Attempt to use action in menu (for use with SelectAll).
	*/
	virtual void ExecuteAction(FBattleCharacterData& ActionOwner, TArray<FBattleCharacterData>& Targets);

	/*
	 * Attempt to use action in menu.
	*/
	virtual void ExecuteAction(FBattleCharacterData& ActionOwner, FBattleCharacterData& Target);

	/*
	 * Custom implementation for action's effect
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex);

	/*
	 * Determine if can use a magic ability in menu.
	 */
	virtual bool CanCast(FBattleCharacterData& ActionOwner);

	/*
	 * Determine if can use an action in battle
	 */
	virtual bool CanUse(AFFYBattleCharacter* ActionOwner, int8 Targets);

	/*
	 * Check if meets ATB cost and state conditions
	 */
	virtual bool CanExecute(AFFYBattleCharacter* ActionOwner);

	/*
	 * Calculate cost for performing action for owner to consume.
	 */
	virtual void Consume(AFFYBattleCharacter* ActionOwner, int8 Targets);

	/*
	 * In cases where character has a resource-free use of this action
	 */
	virtual void FreeUse(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets);

	virtual void DebugLocations(FVector Location1, FVector Normal1, FVector Location2, FVector Normal2);

	/*
	 * In cases where action is used on invalid/dead target.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Redirect(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets);
	
};
