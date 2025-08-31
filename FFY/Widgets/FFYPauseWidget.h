// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFYMasterWidget.h"
#include "FFYMenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "FFYPauseWidget.generated.h"

/**
 * 
 */
// We make the class abstract, as we don't want to create
// instances of this, instead we want to create instances
// of our UMG Blueprint subclass.
UCLASS()
class FFY_API UFFYPauseWidget : public UFFYMasterWidget
{
	GENERATED_BODY()

	
protected:
	// Doing setup in the C++ constructor is not as
	// useful as using NativeConstruct.
	virtual void NativeConstruct() override;
	
};
