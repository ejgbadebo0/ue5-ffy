// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FFYAction.h"
#include "FFYActionContainer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemUse, FName, ID, AFFYBattleCharacter*, User, AFFYBattleCharacter*, Target);

class AFFYItemAction;
class AFFYBattleCharacter;
/**
 * 
 */
UCLASS(Blueprintable)
class FFY_API UFFYActionContainer : public UObject
{
	GENERATED_BODY()

public:

	//DELEGATE:
	FOnItemUse OnItemUse;
	//=======

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AFFYAction* LastAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AFFYAction* QueuedAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AFFYAction* ContextCommand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Actions)
	TArray<AFFYAction*> MainCommands;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Actions)
	TArray<AFFYAction*> Magic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Actions)
	AFFYItemAction* Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Actions)
	TArray<AFFYAction*> Passives;

	UFUNCTION(BlueprintCallable, Category = Actions)
	void AddAction(AFFYAction* Action);

	UFUNCTION(BlueprintCallable, Category = Actions)
	void ItemUseEvent(FName ID, AFFYBattleCharacter* User, AFFYBattleCharacter* Target);
	
};
