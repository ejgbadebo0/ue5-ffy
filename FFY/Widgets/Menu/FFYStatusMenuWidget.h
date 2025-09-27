// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/FFYDataEnums.h"
#include "FFY/Widgets/FFYMenuWidget.h"
#include "FFYStatusMenuWidget.generated.h"

class URichTextBlock;
enum class EStatusEffect : uint8;
/**
 * 
 */
UCLASS()
class FFY_API UFFYStatusMenuWidget : public UFFYMenuWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	//Initialized through LoadContext and use to navigate upon Cycle input. 
	

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

	//Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* StrengthValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* ConstitutionValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* MagicValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* SpiritValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* PrecisionValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* DexterityValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* LuckValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UHorizontalBox* StatusEffectsBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UHorizontalBox* WeaknessesBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UHorizontalBox* ResistancesBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UHorizontalBox* CatalystsBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UHorizontalBox* ImmunitiesBox;

public:
	
	virtual void Refresh() override;
	virtual UFFYSelectPartyMemberWidget* GetSelectionWidget_Implementation() override;

	//Used to initialize variables
	virtual void LoadContext_Implementation(FName ContextName) override;
	virtual FText GetInputKeyText_Implementation() override
	{
		return FText::FromString("<img id=\"LeftMouseButton\"></> = Confirm <img id=\"RightMouseButton\"></> = Cancel <img id=\"Q\"></>/<img id=\"E\"></> = Cycle Members");
	}
};
