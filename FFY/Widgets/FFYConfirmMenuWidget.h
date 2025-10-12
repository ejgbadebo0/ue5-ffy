// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Widgets/FFYMenuWidget.h"
#include "Components/RichTextBlock.h"
#include "FFYConfirmMenuWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmOptionSelected, bool, bIsConfirmed);

/**
 * 
 */
UCLASS()
class FFY_API UFFYConfirmMenuWidget : public UFFYMenuWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	//DELEGATE:
	UPROPERTY(BlueprintAssignable)
	FOnConfirmOptionSelected OnConfirmOptionSelected;
	//=========

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	URichTextBlock* MenuTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Confirm, meta = (AllowPrivateAccess = "true"))
	FName ConfirmCallback = NAME_None;
	
	UFUNCTION(BlueprintCallable)
	void SetConfirmCallback(FName CallbackFunctionName);

	UFUNCTION(BlueprintCallable)
	void ExecuteConfirmCallback(bool bIsConfirmed);

	virtual void SetConfirmMenuText_Implementation(const FText& Text) override
	{
		if (MenuTextBlock != nullptr)
		{
			MenuTextBlock->SetText(Text);
		}
	}
};
