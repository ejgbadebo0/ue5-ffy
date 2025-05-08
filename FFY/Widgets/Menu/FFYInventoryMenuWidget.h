// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Widgets/FFYMenuScrollWidget.h"
#include "FFYInventoryMenuWidget.generated.h"

class UFFYInventoryItemOptionWidget;
/**
 * 
 */
UCLASS()
class FFY_API UFFYInventoryMenuWidget : public UFFYMenuScrollWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void ClearOptions() override;
	virtual void Refresh() override;

	UFUNCTION()
	void InactiveRefresh();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UFFYSelectPartyMemberWidget* SelectionWidget;

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Widget)
	TSubclassOf<UFFYInventoryItemOptionWidget> ItemButtonClass;

	virtual UFFYSelectPartyMemberWidget* GetSelectionWidget_Implementation() override;

	virtual void SetDefaultTargetGroup_Implementation(bool bIsEnemy, bool Reset) override
	{
		Super::SetDefaultTargetGroup_Implementation(bIsEnemy, Reset);
	}
};
