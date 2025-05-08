// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FFY/FFYWidgetEvents.h"
#include "FFY/Widgets/FFYMenuScrollWidget.h"
#include "FFYSelectPartyMemberWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPartyMemberSelected, UFFYPartyMemberOptionWidget*, CharacterWidget, bool, SelectAll);


class UFFYPartyMemberOptionWidget;
/**
 * Is reused in different menus that contain interaction functionality with party members.
 */
UCLASS()
class FFY_API UFFYSelectPartyMemberWidget : public UFFYMenuScrollWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void ClearOptions() override;
	
	virtual void Refresh() override;

public:
    //DELEGATE:
	UPROPERTY(BlueprintAssignable)
	FOnPartyMemberSelected OnPartyMemberSelected;
	//------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Widget)
	TSubclassOf<UFFYPartyMemberOptionWidget> PartyButtonClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Widget)
	ESlateVisibility DefaultActiveVisibility = ESlateVisibility::Visible;

	UFUNCTION(BlueprintCallable)
	void SetSelectAll(bool Value);

	virtual bool GetSelectAll_Implementation() override
	{
		return bSelectAll;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Widget)
	bool bSelectAll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	bool bCanSelectAll;

	UFUNCTION(BlueprintCallable)
	void ToggleSelectionMode();
	
	virtual void PartyMemberSelect_Implementation(UFFYPartyMemberOptionWidget* CharacterWidget) override;
};

inline void UFFYSelectPartyMemberWidget::ToggleSelectionMode()
{
	if (bCanSelectAll)
	{
		SetSelectAll(!bSelectAll);
	}
}
