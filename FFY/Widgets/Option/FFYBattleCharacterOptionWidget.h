// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Widgets/Option/FFYPartyMemberOptionWidget.h"
#include "FFYBattleCharacterOptionWidget.generated.h"

class UFFYPartyHUDSlotOptionWidget;
class UWrapBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBattleCharacterSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBattleCharacterUnselected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBattleCharacterConfirmed);

/**
 * 
 */
UCLASS()
class FFY_API UFFYBattleCharacterOptionWidget : public UFFYOptionWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;
	
public:
	//DELEGATES:
	FOnBattleCharacterSelected OnBattleCharacterSelected;
	FOnBattleCharacterUnselected OnBattleCharacterUnselected;
	FOnBattleCharacterConfirmed OnBattleCharacterConfirmed;
	//---------
	
	void InitializeFromWidget(UFFYPartyHUDSlotOptionWidget* InWidget);

	UPROPERTY(EditDefaultsOnly)
	bool Enemy;
	
	virtual void OnSelected() override;
	virtual void OnUnselected() override;
	virtual void PerformAction() override;
  

	//CALLBACK:
	UFUNCTION()
	virtual void ShouldDisable(bool bIsEnemy);

	UFUNCTION()
	virtual void OnReset();

	UFUNCTION()
	virtual void OnDestroy();
	//-----------
	
	UFUNCTION(BlueprintCallable)
	void OnSelectedProxy();
	
	UFUNCTION(BlueprintCallable)
	void OnUnselectedProxy();
};
