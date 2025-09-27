// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FFY/FFYDataStructures.h"
#include "FFYBattleResultsWidget.generated.h"

class UWrapBox;
class UFFYBattleEXPSlotWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResultScreenExit);

/**
 * 
 */
UCLASS()
class FFY_API UFFYBattleResultsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	//DELEGATE:
	FOnResultScreenExit OnResultScreenExit;
	//==========
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	UWrapBox* EXPWrapBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	UWrapBox* ItemWrapBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UFFYBattleEXPSlotWidget> EXPSlotClass;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Advance();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void AddEXPSlot(FBattleEXPData EXPData);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void AddItemDropSlot(FName ItemID, int Quantity);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartDisplay();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void EndDisplay();

	UFUNCTION(BlueprintCallable)
	void ExitResultsScreen()
	{
		OnResultScreenExit.Broadcast();
	}
};
