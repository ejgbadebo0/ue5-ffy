// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Widgets/FFYMenuScrollWidget.h"
#include "FFYMagicMenuWidget.generated.h"

class UFFYActionOptionWidget;
class URichTextBlock;
/**
 * 
 */
UCLASS()
class FFY_API UFFYMagicMenuWidget : public UFFYMenuScrollWidget
{
	GENERATED_BODY()


protected:
	UFUNCTION()
	void OnConfirmOptionSelected(bool bIsConfirmed);
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	//Initialized through LoadContext and use to navigate upon Cycle input. 
	

	//needs FBattleCharacterData widget variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<FName, UTexture2D*> PortraitTextureCache;

	//From PartyMemberOption 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))	
	UImage* Portrait;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* LevelValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	URichTextBlock* HPValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	URichTextBlock* MPValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* MaxHPValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* MaxMPValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* EXPValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* ReqEXPValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UFFYSelectPartyMemberWidget* SelectionWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UFFYConfirmMenuWidget* ConfirmMenuWidget;

	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Widget)
	TSubclassOf<UFFYActionOptionWidget> ActionOptionClass;
	
	virtual void LoadContext_Implementation(FName ContextName) override;
	
	virtual UFFYSelectPartyMemberWidget* GetSelectionWidget_Implementation() override;
	
	virtual UFFYConfirmMenuWidget* GetConfirmWidget_Implementation() override;
	
	virtual void ContextRefresh_Implementation(int Index) override;
	
	virtual FText GetInputKeyText_Implementation() override
	{
		return FText::FromString("<img id=\"LeftMouseButton\"></> = Confirm <img id=\"RightMouseButton\"></> = Cancel <img id=\"Q\"></>/<img id=\"E\"></> = Cycle Members <img id=\"C\"></> = Select All");
	}

protected:
	virtual void Refresh() override;
};
