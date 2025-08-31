// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFYPartyMemberOptionWidget.h"
#include "FFY/Widgets/FFYOptionWidget.h"
#include "FFYPartyHUDSlotOptionWidget.generated.h"

class AFFYAction;
struct FBattleCharacterData;
class UProgressBar;
class URichTextBlock;
class AFFYBattleCharacter;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPartyHUDSlotSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPartyHUDSlotUnselected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPartyHUDSlotConfirmed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPartyHUDSlotDestroyed);
/**
 * 
 */
UCLASS()
class FFY_API UFFYPartyHUDSlotOptionWidget : public UFFYPartyMemberOptionWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UProgressBar* ATBProgressBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UProgressBar* ATBCostBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UProgressBar* ATBContextCostBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Widget)
	bool ATBVariableCost = false;

	UFUNCTION()
	void OnActionSelected(AFFYAction* Action, bool bIsContext, float Duration);

	UFUNCTION()
	void UpdateATBProgress(float Value);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateATBProgress(float Value);

public:

	//DELEGATES:
	FOnPartyHUDSlotSelected OnPartyHUDSlotSelected;
	FOnPartyHUDSlotUnselected OnPartyHUDSlotUnselected;
	FOnPartyHUDSlotConfirmed OnPartyHUDSlotConfirmed;
	FOnPartyHUDSlotDestroyed OnPartyHUDSlotDestroyed;
	//---

	UFUNCTION()
	void BattleStatsChanged(FBattleCharacterData& NewData);
	
	virtual void OnSelected() override;
	virtual void OnUnselected() override;
	virtual void PerformAction() override;
	virtual UFFYOptionWidget* Flicker_Implementation(bool Value) override;
	UFUNCTION()
	void OnSelectedProxy();
	UFUNCTION()
	void OnUnselectedProxy();
	UFUNCTION()
	void PerformActionProxy();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFocusedModeChanged(AFFYBattleCharacter* Character, bool bIsFocused);
	
	UFUNCTION(BlueprintCallable)
	void InitializeCharacter(AFFYBattleCharacter* CharacterReference);

};
