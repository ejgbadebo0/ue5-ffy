// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "FFYOptionWidget.generated.h"

class UFFYMenuWidget;
class UHorizontalBox;
class UTextBlock;
class UButton;
class UImage;
/**
 * 
 */
UCLASS()
class FFY_API UFFYOptionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//for flicker
	float FlickTime;

	bool bIsHovered;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	ESlateVisibility DefaultUnselectedVisibility = ESlateVisibility::Collapsed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	UFFYMenuWidget* OwnerMenu;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	UTextBlock* DisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	UButton* Button;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	UHorizontalBox* HorizontalBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	UImage* SelectedImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDisabled = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsInitialized = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsFlickering = false;
	
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	FText DisplayTextLabel = FText::GetEmpty();

	UFUNCTION(BlueprintCallable)
	virtual FText GetDescription();
	
	void SetOwnerMenu(UFFYMenuWidget* Menu) { OwnerMenu = Menu; };

	UFFYMenuWidget* GetOwnerMenu() { return OwnerMenu; };

	UFUNCTION(BlueprintCallable)
	virtual void SetIsDisabled(bool Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UFFYOptionWidget* Flicker(bool Value);
	
	virtual void Unflicker();
	
	FTimerHandle FlickerTimerHandle;
	
	UFUNCTION()
	virtual void OnSelected(); 

	UFUNCTION()
	virtual void OnUnselected(); 
	
	UFUNCTION()
	virtual void PerformAction();
};
