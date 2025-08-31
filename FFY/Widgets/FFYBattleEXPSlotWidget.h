// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FFY/FFYDataStructures.h"
#include "FFYBattleEXPSlotWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class FFY_API UFFYBattleEXPSlotWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	UTextBlock* CharacterNameTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	UTextBlock* CharacterLVTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	UTextBlock* TotalEXPGainedTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	UTextBlock* CharacterEXPTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	UTextBlock* CharacterRequiredEXPTextBlock;

	UFUNCTION(BlueprintImplementableEvent)
	void InitializeSlot(FBattleEXPData Data);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartDisplay();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void EndDisplay();

	
};
