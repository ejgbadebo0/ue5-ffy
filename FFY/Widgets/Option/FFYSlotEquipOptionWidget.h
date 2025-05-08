// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "FFY/Widgets/FFYOptionWidget.h"
#include "FFYSlotEquipOptionWidget.generated.h"

enum class EEquipmentSlot : uint8;
enum class EEquipmentClass : uint8;
/**
 * 
 */
UCLASS()
class FFY_API UFFYSlotEquipOptionWidget : public UFFYOptionWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;

public:

	UFUNCTION(BlueprintCallable)
	void SetEquipmentName(FName EquipmentName);
	
	virtual FText GetDescription() override;
	virtual void OnSelected() override;
	virtual void OnUnselected() override;
	virtual void PerformAction() override;
	virtual UFFYOptionWidget* Flicker_Implementation(bool Value) override;

	UPROPERTY(EditAnywhere)
	EEquipmentClass EquipmentClass;
	
	virtual void Unflicker() override;

	UPROPERTY(EditAnywhere)
	EEquipmentSlot EquipmentSlot;
};

inline void UFFYSlotEquipOptionWidget::Unflicker()
{
	if (SelectedImage)
	{
		if (SelectedImage->IsVisible())
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, "On");
			SelectedImage->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, "On");
			SelectedImage->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
