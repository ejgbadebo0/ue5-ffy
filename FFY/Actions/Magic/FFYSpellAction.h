// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FFY/FFYAction.h"
#include "FFYSpellAction.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API AFFYSpellAction : public AFFYAction
{
	GENERATED_BODY()


public:
	virtual void ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets) override;
	virtual void ExecuteAction(FBattleCharacterData& ActionOwner, TArray<FBattleCharacterData>& Targets) override;
	virtual void ExecuteAction(FBattleCharacterData& ActionOwner, FBattleCharacterData& Target) override;
	virtual void DebugLocations(FVector Location1, FVector Normal1, FVector Location2, FVector Normal2) override;
};
