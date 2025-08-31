// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FFYTransitions.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFFYTransitions : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FFY_API IFFYTransitions
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnPossessed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartTransition(FName Signature);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EndTransition(FName Signature);
};
