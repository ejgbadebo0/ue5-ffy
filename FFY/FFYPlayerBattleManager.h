// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
//ext import
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "FFYBattleCharacter.h"
#include "FFYCameraControls.h"
#include "Widgets/FFYBattleWidget.h"
#include "Widgets/FFYChainWidget.h"
//---
#include "FFYPlayerBattleManager.generated.h"


class UFFYBattleResultsWidget;
class UFFYChainWidget;
class UFFYActionWidget;
class UFFYBattleWidget;
class UFFYMenuWidget;
class UFFYMasterWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveModeSwitched, EActiveState, ActiveState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSceneDarken, bool, Active);

UCLASS()
class FFY_API AFFYPlayerBattleManager : public APawn, public IFFYBattleEvents, public IFFYCameraControls
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	float PartyEXPGained;

	UPROPERTY(VisibleAnywhere)
	float ChainBonus;

	UPROPERTY(VisibleAnywhere)
	TMap<FName, int> ItemDrops;

	UPROPERTY(VisibleAnywhere)
	FTimerHandle VictoryTimer;

	UPROPERTY(VisibleAnywhere)
	int CurrentCameraPriority = 0;


public:
	// Sets default values for this pawn's properties
	AFFYPlayerBattleManager();

	//DELEGATE:
	UPROPERTY(BlueprintAssignable)
	FOnActiveModeSwitched OnActiveModeSwitched;
	UPROPERTY(BlueprintAssignable)
	FOnSceneDarken OnSceneDarken;

	//WIDGETS:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UFFYMasterWidget> MasterWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UFFYActionWidget> ActionWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UFFYChainWidget> ChainWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UFFYBattleResultsWidget> BattleResultsWidgetClass;
		
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	UFFYBattleWidget* BattleWidget;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	UFFYActionWidget* ActionWidget;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	UFFYChainWidget* ChainWidget;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	UFFYBattleResultsWidget* BattleResultsWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	UFFYMenuWidget* ActiveMenuContext;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	//AXIS:
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CycleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ContextCommandAction; 

	//ACTION:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ConfirmAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CancelAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SelectModeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchTargetsAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DefendAction;



	//INPUT FUNCTIONS:
	void Move(const FInputActionValue& Value);

	void Confirm(const FInputActionValue& Value);
	
	void Cancel(const FInputActionValue& Value);

	void Cycle(const FInputActionValue& Value);
	
	void Pause(const FInputActionValue& Value);

	void SelectMode(const FInputActionValue& Value);

	void SwitchTargets(const FInputActionValue& Value);

	void Defend(const FInputActionValue& Value);

	void ContextCommand(const FInputActionValue& Value);

	//--------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* MainCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, Category = "Characters", meta = (AllowPrivateAccess = "true"))
	int PartyIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Battle", meta = (AllowPrivateAccess = "true"))
	bool bHasBattleStarted = false;

	UPROPERTY(EditAnywhere, Category = "Battle", meta = (AllowPrivateAccess = "true"))
	EActiveState ActiveState = EActiveState::ACTIVE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battle", meta = (AllowPrivateAccess = "true"))
	FTransform DefaultTransform;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battle", meta = (AllowPrivateAccess = "true"))
	TArray<FTransform> BattleCameraTransforms;

protected:
	
	UFUNCTION()
	void CheckLossCondition(AFFYBattleCharacter* Character);

	//called when enemy dies
	UFUNCTION()
	void CheckWinCondition(AFFYBattleCharacter* Character);

	UFUNCTION()
	void StartVictory();

	UFUNCTION()
	void Victory();

	UFUNCTION()
	void StartDefeat();
	
	UFUNCTION()
	void Defeat();

	UFUNCTION(BlueprintCallable)
	void BattleEnded();

	UFUNCTION()
	FBattleEXPData CalculateEXPGained(AFFYBattleCharacter* Character);

	UFUNCTION()
	void LevelUp(AFFYBattleCharacter* Character, FBattleEXPData& EXPData);

	
	UFUNCTION(BlueprintCallable)
	void ResetBattleActiveState(EActionState NewActionState);
	
	//used when character takes action that requires Wait state
	UFUNCTION()
	void SetBattleActiveState(AFFYBattleCharacter* Character);

	UFUNCTION(BlueprintCallable)
	void SetAllWaitMode(AFFYBattleCharacter* Character);

	UFUNCTION()
	void OnCameraActionSelected(AFFYBattleCharacter* Character, FCameraActionContainer CameraActionContainer);

	UFUNCTION(BlueprintCallable)
	void OnCameraActionFinished()
	{
		CurrentCameraPriority = 0;
		bCameraActionOverride = false;
	}

	UFUNCTION(BlueprintCallable)
	void ChangeDefaultTransform(bool ForceChange = false);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when battle starts to determine initial ATB for each actor
	float FindHighestDexterity() const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//called when list of enemies is finalized
	UFUNCTION(BlueprintCallable, Category = "Characters")
	void UpdateEnemies(TArray<AFFYBattleCharacter*> LoadedEnemies);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters", meta = (AllowPrivateAccess = "true"))
	TArray<AFFYBattleCharacter*> Enemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters", meta = (AllowPrivateAccess = "true"))
	TArray<AFFYBattleCharacter*> Party;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	FDataTableRowHandle LevelUpDataTableHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	bool bCameraActionOverride = false;

	bool ChainActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle", meta = (AllowPrivateAccess = "true"))
	int CurrentChain = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Battle", meta = (AllowPrivateAccess = "true"))
	float FallOffValue = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle", meta = (AllowPrivateAccess = "true"))
	float CurrentChainFallOffRate = 1.f;

	//all characters present
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle", meta = (AllowPrivateAccess = "true"))
	TArray<FCameraActionContainer> VictoryCameraActions;

	//with some characters defeated
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle", meta = (AllowPrivateAccess = "true"))
	TArray<FCameraActionContainer> VictoryWCDCameraActions;

	UFUNCTION()
	virtual void AssignInitiative(); 
	
	UFUNCTION()
	virtual void OnStartBattle();

	//INTERFACE:

	virtual void ActionUsed_Implementation(FName ActionName, bool bIsEnemy) override;

	virtual void AddFallOff_Implementation(float FallOff) override
	{
		FallOffValue += FallOff;
		
		if (ChainWidget)
		{
			ChainWidget->AddFallOff_Implementation(FallOff);
		}
		
	}

	virtual int GetChain_Implementation() override
	{
		return CurrentChain;
	}

	virtual void ResetChain_Implementation() override
	{
		ChainActive = false;
		CurrentChain = 0;
		
		if (ChainWidget)
		{
			ChainWidget->ResetChain_Implementation();
		}
	}

	virtual void UpdateChain_Implementation(int Amount, float FallOffRate) override
	{
		ChainActive = true;
		CurrentChain = Amount;
		FallOffValue = 100.f;
		CurrentChainFallOffRate = FallOffRate;
		
		if (ChainWidget)
		{
			ChainWidget->UpdateChain_Implementation(Amount, FallOffRate);
		}
	}

	//deltatime for actor, final value in widget
	virtual void UpdateFallOff_Implementation(float Value) override
	{
		if (!ChainActive)
		{
			return;
		}
		FallOffValue -= FMath::Clamp((3.f * Value * CurrentChainFallOffRate), 0.f, 100.f);
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, FString::Printf(TEXT("Chain: %i, FO: %f"), CurrentChain, FallOffValue));
		if (ChainWidget)
		{
			ChainWidget->UpdateFallOff_Implementation((FallOffValue/100.f));
			if (FallOffValue <= 0.f)
			{
				ResetChain_Implementation();
			}
		}
	}

	virtual void DarkenScene_Implementation(bool Active) override
	{
		OnDarkenScene(Active);  //post process effects
		OnSceneDarken.Broadcast(Active); //signal to listening actors for custom behavior
	}


	virtual void EndIntroSequence_Implementation(FTransform CurrentSequenceTransform) override
	{
		OnEndIntroSequence(CurrentSequenceTransform);
	}

	//===========

	//BP EVENTS:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnCameraActionStarted(AFFYBattleCharacter* Character, FCameraActionContainer CameraActionContainer);

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndIntroSequence(FTransform CurrentSequenceTransform);

	UFUNCTION(BlueprintImplementableEvent)
	void StopOngoingCameraAction();

	UFUNCTION(BlueprintImplementableEvent)
	void StartVictoryCameraAction();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFocusStateChanged(AFFYBattleCharacter* Character, bool bIsFocused);

	UFUNCTION(BlueprintImplementableEvent)
	void OnClashEvent(AFFYBattleCharacter* Character);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDarkenScene(bool Active);

	UFUNCTION(BlueprintImplementableEvent)
	void OnGameOver();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Audio")
	FMusicData VictoryMusicData;

	//===========
};
