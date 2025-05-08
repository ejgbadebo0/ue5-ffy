// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Widgets/Menu/FFYSelectPartyMemberWidget.h"
#include "FFY/Widgets/Option/FFYPartyHUDSlotOptionWidget.h"
#include "FFYSelectBattleCharacterWidget.generated.h"

class UBorder;
class UFFYEnemySlotOptionWidget;
class UFFYPartyHUDSlotOptionWidget;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOptionGroupDisabled, bool, bIsEnemy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResetAll);
/**
 * 
 */
UCLASS()
class FFY_API UFFYSelectBattleCharacterWidget : public UFFYSelectPartyMemberWidget
{
	GENERATED_BODY()



public:

	//DELEGATES:
	FOnOptionGroupDisabled OnOptionGroupDisabled;
	FOnResetAll OnResetAll;
	//------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	bool bIsTargetingEnemies = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget", meta = (BindWidget))
	UBorder* PartyHUD;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget", meta = (BindWidget))
	UBorder* EnemyHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (BindWidget))
	UWrapBox* EnemyList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TArray<UFFYOptionWidget*> PartyOptions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TArray<UFFYOptionWidget*> EnemyOptions;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UFFYEnemySlotOptionWidget> EnemySlotWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void AddHUDSlot(AFFYBattleCharacter* Character);

	//INTERFACE:
	virtual void SetDefaultTargetGroup_Implementation(bool bIsEnemy, bool Reset) override;

	virtual bool GetDefaultTargetGroup_Implementation() override
	{
		return bIsTargetingEnemies;
	}

	virtual TArray<AFFYBattleCharacter*> GetTargetGroupActors_Implementation(bool bIsEnemies) override;


	virtual void ResetOptions_Implementation() override;
	//virtual void PartyMemberSelect_Implementation(UFFYPartyMemberOptionWidget* CharacterWidget) override;

protected:
	virtual void NativeConstruct() override;
	virtual void Refresh() override;
};