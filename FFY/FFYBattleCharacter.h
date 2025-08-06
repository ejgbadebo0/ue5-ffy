// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//ext includes
#include "FFYAction.h"
#include "FFYActionContainer.h"
#include "FFYBattleEvents.h"
#include "FFYDataStructures.h"
#include "FFYDataEnums.h"
#include "FFYWidgetEvents.h"
#include "MotionWarpingComponent.h"
#include "Engine/StreamableManager.h"
//-------
#include "FFYBattleCharacter.generated.h"

class UNiagaraComponent;
class UFFYGambitComponent;
class UFFYBattleCharacterOptionWidget;
class UWidgetComponent;
class UFFYActionContainer;
struct FStreamableHandle;
struct FPartySlot;
class UAnimInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterStatsChanged, FBattleCharacterData&, NewData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnATBValueChanged, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionSelected, AFFYAction*, Action, bool, bIsContext);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnContextCommandActivated, AFFYBattleCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnContextCommandDeactivated, AFFYBattleCharacter*, Character);

//used to map status effects to disabled functions
USTRUCT(BlueprintType)
struct FFY_API FAffectedActionTypes
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EActionType> ActionTypes;
};

//used to display info about damage event results
USTRUCT(BlueprintType)
struct FFY_API FDamageEventResult
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bWasHit = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsHealing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPerfectDefense = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageReceived = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ResultDescription = FText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDamageAttributes InDamageAttributes = FDamageAttributes();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SourceLocation = FVector::ZeroVector;
};

UCLASS()
class FFY_API AFFYBattleCharacter : public ACharacter, public IFFYWidgetEvents, public IFFYBattleEvents
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFFYBattleCharacter();

	//DELEGATES:
	FOnCharacterStatsChanged OnCharacterStatsChanged;
	FOnATBValueChanged OnATBValueChanged;
	FOnCharacterSelected OnCharacterSelected;
	FOnActionSelected OnActionSelected;
	FOnContextCommandActivated OnContextCommandActivated;
	FOnContextCommandDeactivated OnContextCommandDeactivated;
	

	//Inherited Variables: 
	//----------------------

	//Actor Variables: 
	//----------------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI", meta=(AllowPrivateAccess=true))
	UTexture2D* PortraitImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	FBattleCharacterData BattleCharacterStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UFFYGambitComponent* GambitComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UMotionWarpingComponent* MotionWarpingComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* SelectionWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Niagara", meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* NiagaraSystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UFFYBattleCharacterOptionWidget* SelectionWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* HUDWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* DamageEventsWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	FName WeaponSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	FName ShieldSocketName;

	TSharedPtr<FStreamableHandle> WeaponHandle;
	
	TSharedPtr<FStreamableHandle> ShieldHandle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ShieldMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimInstance* AnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::IDLE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Battle")
	TArray<AFFYAction*> Actions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Battle")
	UFFYActionContainer* ActionContainer;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Battle")
	TArray<AFFYBattleCharacter*> Targets;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Battle")
	float ATB = 0;

	int8 Initiative = FMath::RandRange(0, 20);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Battle")
	float DefenseFactor;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Battle")
	bool PerfectDefense = false;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Battle")
	FTimerHandle PerfectDefenseTimer;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Battle")
	EElement Infusion = EElement::NONE;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Battle")
	FDamageAttributes DamageAttributes;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Battle")
	float DefaultTimeDilation = 1.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	/* If this is a party member, attempt to find the data in GameInstance class. */
	void InitializeFromPartySlot();

	/* If this is an enemy, find BattleCharacterStats from Data table. */
	void InitializeFromEnemyDataTable();

	/* Callback from chain delegate */
	UFUNCTION()
	void ItemUseEvent(FName ID, AFFYBattleCharacter* User, AFFYBattleCharacter* Target);
	/*
	 * To be called when spawned
	 */
	void InitBattleCharacterStats(FBattleCharacterData& PartyCharacterData, TArray<TSubclassOf<AFFYAction>> AbilityClasses, UFFYActionContainer* Container);

	/*
	 * Called in constructor
	 */
	void InitActorVariables();

	/*
	 * Used for enemy classes.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defaults")
	TArray<TSubclassOf<AFFYAction>> DefaultAbilityClasses;
	
	/*
	 * Used for enemy classes.
	 */
	void InitDefaultAbilities();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Manager needs to be persistently referenced to ensure assets finish loading before function stack is culled.
	FStreamableManager StreamableManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (RowType = "BattleCharacterData"))
	FDataTableRowHandle EnemyDataHandle;

	UFUNCTION(BlueprintCallable)
	void OnWeaponLoaded();

	UFUNCTION(BlueprintCallable)
	void OnShieldLoaded();

	//STATIC DEFAULTS:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Battle")
	TMap<EStatusEffect, float> TimeAlteringEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Battle")
	TMap<EStatusEffect, FAffectedActionTypes> ActionDisablingEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Battle")
	TArray<EStatusEffect> UndamageableEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Battle")
	TMap<EStatusEffect, EDamageModifier> ResistanceTypes;

	float DamageCap = 9999.f;
	/*
	* Set on BeginPlay
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defaults")
	FTransform DefaultTransform;
	
	//-------------

	UFUNCTION(BlueprintCallable)
	void Consume(float ATBCost, float HPCost, float MPCost);

	UFUNCTION(BlueprintCallable)
	void UpdateATB(float DeltaTimeX);

	void UpdateDamageAttributes(float DamageAmount = 0,
		EStat DamageStat = EStat::HP,
		EDamageModifier DamageType = EDamageModifier::NONE,
		EElement DamageElement = EElement::NONE,
		TArray<EStatusEffect> Inflicts = TArray<EStatusEffect>(),
		TArray<EStatusEffect> Removes = TArray<EStatusEffect>(),
		EAttackType AttackType = EAttackType::NONE );

	void UpdateMotionWarpTransform(FName WarpName = NAME_None, FVector WarpLocation = FVector::ZeroVector, FRotator WarpRotation = FRotator::ZeroRotator) const;
	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void EndPerfectDefense()
	{
		PerfectDefense = false;
		GetWorld()->GetTimerManager().ClearTimer(PerfectDefenseTimer);
	};

	//INTERFACE ========================================:
    // --- Widget:
	virtual void MenuActionSelected_Implementation(AFFYAction* SelectedAction) override
	{
		OnActionSelected.Broadcast(SelectedAction, false);
	}
	// --- Battle:

	// BP EVENTS: 
	UFUNCTION(BlueprintImplementableEvent)
	void DamageTakenEvent(FDamageEventResult DamageEventResult);
	
	UFUNCTION(BlueprintImplementableEvent)
	void DamageDealtEvent(bool bIsSuccess);

	UFUNCTION(BlueprintImplementableEvent)
	void StatusInflictedEvent(EStatusEffect StatusEffect);

	UFUNCTION(BlueprintImplementableEvent)
	void StatusRemovedEvent(EStatusEffect StatusEffect);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPerfectEvade();

	//==========================
	
	FDamageEventResult InflictDamage(const FDamageAttributes& SourceDamage, int SourceLevel);

	virtual bool ReceiveDamage_Implementation(AFFYBattleCharacter* Source, AFFYAction* SourceAction) override;

	virtual bool ReceiveHealing_Implementation(AFFYBattleCharacter* Source, AFFYAction* SourceAction) override;

	virtual bool GetIsDead_Implementation() override
	{
		return BattleCharacterStats.StatusEffects.Contains(EStatusEffect::KO);
	}

	virtual FDamageAttributes GetDamageAttributes_Implementation() override
	{
		return DamageAttributes;
	}

	virtual void OnStartDefendAction_Implementation(float ATBValue) override
	{
		DefenseFactor = ATBValue/100;
		if (DefenseFactor >= 0.5)
		{
			PerfectDefense = true;
			GetWorld()->GetTimerManager().SetTimer(PerfectDefenseTimer, this, &AFFYBattleCharacter::EndPerfectDefense, 0.5f, false);
		}
	}

	UFUNCTION(BlueprintImplementableEvent)
	void OnBattleEffectSpawnEffect(TSubclassOf<AFFYBattleEffect> BattleEffectClass);

	virtual void BeginSpawnBattleEffect_Implementation(TSubclassOf<AFFYBattleEffect> BattleEffectClass) override
	{
		OnBattleEffectSpawnEffect(BattleEffectClass);
	}

	virtual void TriggerBattleEffect_Implementation() override
	{
		if (ActionContainer)
		{
			if (ActionContainer->LastAction)
			{
				ActionContainer->LastAction->Effect(this, Targets[0]);
			}
		}
	}

	virtual void DisableContextCommand_Implementation() override
	{
		if (ActionContainer)
		{
			OnContextCommandDeactivated.Broadcast(this);
			ActionContainer->ContextCommand = nullptr;
			OnActionSelected.Broadcast(nullptr, true);
		}
	}

	virtual void EnableContextCommand_Implementation(FName ActionName) override
	{
		if (ActionContainer)
		{
			for (auto a : ActionContainer->MainCommands)
			{
				if (a)
				{
					if (a->Label == ActionName)
					{
						ActionContainer->ContextCommand = a;
						OnContextCommandActivated.Broadcast(this);
						OnActionSelected.Broadcast(ActionContainer->ContextCommand, true);
						return;
					}
				}
			}
		}
	}

	void ExecuteGambitCommand(FName ActionName, const TArray<AFFYBattleCharacter*> ActionTargets)
	{
		if (ActionContainer)
		{
			bool FoundAction = false;
			//Is it a Main command
			for (auto a : ActionContainer->MainCommands)
			{
				if (a)
				{
					if (a->Label == ActionName)
					{
						a->ExecuteAction(this, ActionTargets);
						FoundAction = true;
						break;
					}
				}
			}

			if (FoundAction)
			{
				return;
			}
			//Is it a Magic command
			for (auto a : ActionContainer->Magic)
			{
				if (a)
				{
					if (a->Label == ActionName)
					{
						a->ExecuteAction(this, ActionTargets);
						break;
					}
				}
			}
		}
	}

	virtual void ActionUsed_Implementation(FName ActionName, bool bIsEnemy) override;


	virtual FName GetItemName_Implementation(FName ID) override
	{
		IFFYBattleEvents* GameInstance = Cast<IFFYBattleEvents>(GetWorld()->GetGameInstance());
		if (GameInstance)
		{
			return GameInstance->GetItemName_Implementation(ID);
		}
		else
		{
			return NAME_None;
		}
	}
	
};
