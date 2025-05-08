// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FFYInteraction.generated.h"

class AFFYInteractableActor;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFFYInteraction : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FFY_API IFFYInteraction
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateInteractActor(AFFYInteractableActor* InteractActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool Interact(AFFYInteractableActor* InteractActor);


	
	
};
