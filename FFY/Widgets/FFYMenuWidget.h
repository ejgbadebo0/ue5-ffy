// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//ext includes
#include "FFYMasterWidget.h"
#include "FFYOptionWidget.h"
#include "FFY/FFYWidgetEvents.h"
//----
#include "FFYMenuWidget.generated.h"

class UFFYSelectPartyMemberWidget;
enum class EMenuMode : uint8;
enum class ETargetType : uint8;
class UFFYMasterWidget;
class UImage;
class UWrapBox;
class UCanvasPanel;
/**
 * 
 */



UCLASS()
class FFY_API UFFYMenuWidget : public UUserWidget, public IFFYWidgetEvents
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	UFFYMasterWidget* OwnerMaster;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsInitialized = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	bool bHasCustomSelectionWidget = true;

	int ContextIndex = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	UWrapBox* WrapBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	UImage* DisablePanel;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* DescriptionTextBlock;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TArray<UFFYOptionWidget*> Options;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TArray<TSubclassOf<UFFYOptionWidget>> OptionClasses;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	UFFYOptionWidget* CurrentOption;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	EMenuMode CurrentMenuMode;

public:
	UFUNCTION(BlueprintCallable)
	virtual void Refresh();

	UFUNCTION(BlueprintCallable)
	virtual void ClearOptions();

	UFUNCTION(BlueprintCallable)
	TArray<UFFYOptionWidget*> GetOptions() { return Options; };
	
	void SetOwnerMaster(UFFYMasterWidget* Master) { OwnerMaster = Master; };

	UFFYMasterWidget* GetOwnerMaster() { return OwnerMaster; };

	void SetDescription(const FText& Description) { if (DescriptionTextBlock) { DescriptionTextBlock->SetText(Description); } };

	UFUNCTION(BlueprintCallable, Category = UI)
	void SetCurrentOption(UFFYOptionWidget* Option) { CurrentOption = Option; if (DescriptionTextBlock) { DescriptionTextBlock->SetText(CurrentOption->GetDescription()); } };

	EMenuMode GetCurrentMenuMode() { return CurrentMenuMode; };

	void SetMenuMode(EMenuMode Mode) { OwnerMaster->SetMenuMode(Mode); CurrentMenuMode = Mode; };
	
	void NavigateToMenu(FName Menu, FName ContextName);

	void CycleContextIndex(int DeltaIndex);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UFFYSelectPartyMemberWidget* GetSelectionWidget();

	UFUNCTION(BlueprintCallable)
	void ExecuteContextAction(UFFYPartyMemberOptionWidget* CharacterWidget, bool SelectAll);
	
	//INTERFACE:
	virtual void ResetOptions_Implementation() override;
	
	virtual void PartyMemberSelect_Implementation(UFFYPartyMemberOptionWidget* CharacterWidget) override;
	
	virtual void SetDefaultTargetGroup_Implementation(bool bIsEnemy, bool Reset) override;
	
	virtual void EndSelection_Implementation() override;
	
	virtual void StartSelection_Implementation(UFFYOptionWidget* SelectedOption, ETargetType SelectedTargetType) override;

	// ----- INPUTS:
	virtual void SelectAllInputEvent_Implementation() override;
	
	virtual void CycleInputEvent_Implementation(int DeltaIndex) override;

	virtual void SwitchTargetsInputEvent_Implementation() override;

	// --------------
	virtual FText GetInputKeyText_Implementation() override
	{
		return FText::FromString("<img id=\"Tab\"></> = Unpause <img id=\"LeftMouseButton\"></> = Confirm <img id=\"RightMouseButton\"></> = Cancel");
	}

	//virtual void LoadContext_Implementation(FName ContextName) override;

	//virtual void ContextAction_Implementation(UFFYPartyMemberOptionWidget* CharacterWidget) override;
};

/*
 * Execute contextual logic based on selected party member widget and previously selected button.
 */
inline void UFFYMenuWidget::ExecuteContextAction(UFFYPartyMemberOptionWidget* CharacterWidget, bool SelectAll)
{
	IFFYWidgetEvents* ContextOption = Cast<IFFYWidgetEvents>(CurrentOption);
	if (ContextOption)
	{
		ContextOption->ContextAction_Implementation(CharacterWidget, SelectAll);
	}
}
