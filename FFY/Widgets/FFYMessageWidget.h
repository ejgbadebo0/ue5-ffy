// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FFYMessageWidget.generated.h"

class AFFYCharacter;
class URichTextBlock;
/**
 * 
 */
UCLASS()
class FFY_API UFFYMessageWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AFFYCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	URichTextBlock* MessageTextBlock;

	int CurrentMessageIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (ExposeOnSpawn=true))
	TArray<FText> Messages;

	UFUNCTION(BlueprintCallable, Category = UI)
	void AdvanceMessage();
	
};
