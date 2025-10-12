// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FFYAnimationControls.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFFYAnimationControls : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FFY_API IFFYAnimationControls
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayActionMontage(FName Signature, bool bIsMultiTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetOverrideIdleAnimation(bool bIsActive);
	
};
