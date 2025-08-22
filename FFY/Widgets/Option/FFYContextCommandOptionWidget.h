// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Widgets/FFYOptionWidget.h"
#include "FFYContextCommandOptionWidget.generated.h"


class URichTextBlock;
class AFFYBattleCharacter;
/**
 * 
 */
UCLASS()
class FFY_API UFFYContextCommandOptionWidget : public UFFYOptionWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AFFYBattleCharacter* CharacterReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	UImage* Portrait;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	UImage* PressedImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	UTextBlock* CommandTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	URichTextBlock* InputTextBlock;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = UI)
	FText DefaultInputText;

	UFUNCTION(BlueprintCallable)
	virtual void Reload(AFFYBattleCharacter* Character);

	UFUNCTION(BlueprintCallable)
	virtual void Unload();

	UFUNCTION(BlueprintImplementableEvent)
	void OnReload();

	UFUNCTION(BlueprintImplementableEvent)
	void OnUnload();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPressed();

	virtual void PerformAction() override;
};
