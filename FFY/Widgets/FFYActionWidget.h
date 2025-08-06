// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FFYActionWidget.generated.h"

/**
 * 
 */
UCLASS()
class FFY_API UFFYActionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnActionUsed(FName ActionName, bool bIsEnemy);
	
};
