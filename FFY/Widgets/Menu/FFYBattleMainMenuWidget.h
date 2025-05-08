// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Widgets/FFYMenuWidget.h"
#include "FFYBattleMainMenuWidget.generated.h"

class UFFYItemsOptionWidget;
class UFFYBattleMagicOptionWidget;
class UFFYBattleActionOptionWidget;
/**
 * 
 */
UCLASS()
class FFY_API UFFYBattleMainMenuWidget : public UFFYMenuWidget
{
	GENERATED_BODY()

public:
	virtual void LoadBattleContext_Implementation(AFFYBattleCharacter* Character) override;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UFFYBattleActionOptionWidget> ActionOptionWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UFFYBattleMagicOptionWidget> MagicOptionWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UFFYItemsOptionWidget> ItemsOptionWidgetClass;
	
	virtual FText GetInputKeyText_Implementation() override
	{
		return FText::FromString("<img id=\"LeftMouseButton\"></> = Confirm\n \n <img id=\"RightMouseButton\"></> = Cancel\n \n <img id=\"Q\"></>/<img id=\"E\"></> = Cycle Members\n \n");
	}

	virtual UFFYSelectPartyMemberWidget* GetSelectionWidget_Implementation() override;

	virtual void SwitchTargetsInputEvent_Implementation() override
	{
		Super::SwitchTargetsInputEvent_Implementation();
	}

	virtual void SetDefaultTargetGroup_Implementation(bool bIsEnemy, bool Reset) override;
};
