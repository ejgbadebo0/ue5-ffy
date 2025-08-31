// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/FFYBattleCharacter.h"
#include "FFY/Widgets/FFYMenuWidget.h"
#include "FFY/Widgets/Option/FFYContextCommandOptionWidget.h"
#include "FFYContextCommandMenuWidget.generated.h"

class UFFYContextCommandOptionWidget;
/**
 * 
 */
UCLASS()
class FFY_API UFFYContextCommandMenuWidget : public UFFYMenuWidget
{
	GENERATED_BODY()

protected:
	
	virtual void NativeConstruct() override;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	UFFYContextCommandOptionWidget* LeftInputCCWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	UFFYContextCommandOptionWidget* RightInputCCWidget;

	UFUNCTION()
	void LoadContextCommand(AFFYBattleCharacter* Character);
	
	UFUNCTION()
	void UnloadContextCommand(AFFYBattleCharacter* Character);
	
	UFUNCTION(BlueprintCallable, Category = "References")
	void InitializeParty(TArray<AFFYBattleCharacter*> Party)
	{
		for (auto p : Party)
		{
			if (p)
			{
				p->OnContextCommandActivated.AddUniqueDynamic(this, &UFFYContextCommandMenuWidget::LoadContextCommand);
				p->OnContextCommandDeactivated.AddUniqueDynamic(this, &UFFYContextCommandMenuWidget::UnloadContextCommand);
			}
		}
	}


	//INTERFACE:

	virtual void ContextCommandInputEvent_Implementation(int DeltaIndex) override
	{
		if (DeltaIndex > 0)
		{
			if (RightInputCCWidget->IsVisible())
			{
				RightInputCCWidget->PerformAction();
			}
		}
		else
		{
			if (LeftInputCCWidget->IsVisible())
			{
				LeftInputCCWidget->PerformAction();
			}
		}
	}
	
};
