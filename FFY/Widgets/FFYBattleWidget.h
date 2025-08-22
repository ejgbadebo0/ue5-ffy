// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "FFY/FFYBattleCharacter.h"
#include "FFY/Widgets/FFYMasterWidget.h"
#include "FFYBattleWidget.generated.h"

class UFFYActionQueueMenuWidget;
class UFFYContextCommandMenuWidget;
class UImage;
class UFFYPartyHUDSlotOptionWidget;
class UWrapBox;
/**
 * 
 */
UCLASS()
class FFY_API UFFYBattleWidget : public UFFYMasterWidget
{
	GENERATED_BODY()


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget));
	UImage* ContextPortrait;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget));
	UFFYContextCommandMenuWidget* ContextCommandMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget));
	UFFYActionQueueMenuWidget* ActionQueueMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	AFFYBattleCharacter* BattleContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UFFYPartyHUDSlotOptionWidget> HUDSlotWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void AddHUDSlot(AFFYBattleCharacter* Character);

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void InitializeContextMenu(TArray<AFFYBattleCharacter*> Party);

	
	//EnemySlotClass
	
	virtual void EndSelection_Implementation() override;

	//INTERFACE:
	// ----- INPUTS:
	virtual void CycleInputEvent_Implementation(int DeltaIndex) override;

	virtual void DefendInputEvent_Implementation() override;

	virtual void ContextCommandInputEvent_Implementation(int DeltaIndex) override;
	// -------------

	
	virtual void LoadBattleContext_Implementation(AFFYBattleCharacter* Character) override;

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void RefreshBattleContext()
	{
		LoadBattleContext_Implementation(BattleContext);
	};
	
	UFUNCTION(BlueprintCallable, Category = "References")
	virtual AFFYBattleCharacter* GetBattleContext_Implementation() override { return BattleContext; };
	
	UFUNCTION(BlueprintCallable, Category = "References")
	void SetBattleContext_Implementation(AFFYBattleCharacter* Context) override
	{
		if (Context != nullptr)
		{
			BattleContext = Context;
			BattleContext->OnCharacterSelected.Broadcast();
			OnBattleContextChanged();
		}
	}

	//Override to allow selection of characters in background
	virtual void NavigateToMenu(FName Menu, FName ContextName) override
	{
		Super::NavigateToMenu(Menu, ContextName);
		WidgetSwitcher->GetActiveWidget()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnBattleContextChanged();
	
	virtual void SelectAllInputEvent_Implementation() override;
	virtual UFFYSelectPartyMemberWidget* GetMasterSelectionWidget_Implementation() override;


	//BP EVENTS:

	UFUNCTION(BlueprintImplementableEvent)
	void BattleEnd();

	UFUNCTION(BlueprintImplementableEvent)
	void ActiveModeSwitched(EActiveState ActiveState);
	//======

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;


};
