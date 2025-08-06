// Fill out your copyright notice in the Description page of Project Settings.

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
//---
#include "FFYPlayerBattleManager.generated.h"


class UFFYActionWidget;
class UFFYBattleWidget;
class UFFYMenuWidget;
class UFFYMasterWidget;

UCLASS()
class FFY_API AFFYPlayerBattleManager : public APawn, public IFFYBattleEvents
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFFYPlayerBattleManager();


	//WIDGETS:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UFFYMasterWidget> MasterWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UFFYActionWidget> ActionWidgetClass;
		
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	UFFYBattleWidget* BattleWidget;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	UFFYActionWidget* ActionWidget;

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

	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* MainCamera;

	UPROPERTY(EditAnywhere, Category = "Characters", meta = (AllowPrivateAccess = "true"))
	int PartyIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Battle", meta = (AllowPrivateAccess = "true"))
	bool bHasBattleStarted = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when battle starts to determine initial ATB for each actor
	float FindHighestDexterity() const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Characters")
	void UpdateEnemies(TArray<AFFYBattleCharacter*> LoadedEnemies);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters", meta = (AllowPrivateAccess = "true"))
	TArray<AFFYBattleCharacter*> Enemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters", meta = (AllowPrivateAccess = "true"))
	TArray<AFFYBattleCharacter*> Party;
	
	UFUNCTION()
	virtual void OnStartBattle()
	{
		//Set ATB to initial values based on Dexterity and rolled Initiative
		float HighestDex = FindHighestDexterity();
		for (auto p : Party)
		{
			if (p)  //bind
			{
				p->ATB = FMath::Clamp(100*(p->BattleCharacterStats.Dexterity/HighestDex) - (20-p->Initiative), 0, 99);
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Purple, FString::Printf(TEXT("%s: INITIATIVE = %f"), *p->BattleCharacterStats.CharacterName.ToString(), 100*(p->BattleCharacterStats.Dexterity/HighestDex) - (20-p->Initiative)));
			}
		}
		for (auto e : Enemies)
		{
			if (e)
			{
				e->ATB = FMath::Clamp(100*(e->BattleCharacterStats.Dexterity/HighestDex) - (20-e->Initiative), 0, 99);
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("%s: INITIATIVE = %f"), *e->BattleCharacterStats.CharacterName.ToString(), 100*(e->BattleCharacterStats.Dexterity/HighestDex) - (20-e->Initiative)));
			}
		}
		
		//Enable Battle actions
		bHasBattleStarted = true;
	}

	//INTERFACE:

	virtual void ActionUsed_Implementation(FName ActionName, bool bIsEnemy) override;
	
	//===========
};
