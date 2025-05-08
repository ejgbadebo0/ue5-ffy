// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FFY/FFYAction.h"
#include "FFYAttackAction.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API AFFYAttackAction : public AFFYAction
{
	GENERATED_BODY()
public:
	AFFYAttackAction();
	
	virtual float GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner) override;
	virtual void ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets) override;
	virtual void ExecuteAction(FBattleCharacterData& Owner, TArray<FBattleCharacterData>& Targets) override;
	virtual void Redirect(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets) override;
	virtual void DebugLocations(FVector Location1, FVector Normal1, FVector Location2, FVector Normal2) override;
	virtual void Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target) override;
};
