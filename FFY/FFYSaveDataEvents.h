// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FFYSaveDataEvents.generated.h"

struct FPartySlot;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFFYSaveDataEvents : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FFY_API IFFYSaveDataEvents
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool HasSaveData(FName ID);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	uint8 GetSaveState(FName ID);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetSaveState(FName ID, uint8 State);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void FullHeal();
	
};
