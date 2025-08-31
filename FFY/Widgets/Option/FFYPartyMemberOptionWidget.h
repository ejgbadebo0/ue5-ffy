// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/FFYGameInstance.h"
#include "FFY/Widgets/FFYOptionWidget.h"
#include "FFYPartyMemberOptionWidget.generated.h"

class URichTextBlock;
/**
 * Reads from the current party member slot index in Game Instance and displays it as a button. 
 */
UCLASS()
class FFY_API UFFYPartyMemberOptionWidget : public UFFYOptionWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual FText GetDescription() override
	{
		return Super::GetDescription();
	}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<FName, UTexture2D*> PortraitTextureCache;

	//Set from PartySlot struct
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
	//-----------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
	AFFYBattleCharacter* BattleCharacterReference;
public:

	void InitializeOption(FPartySlot &PartySlot);

	FName GetCharacterName();

	UFUNCTION(BlueprintCallable)
	AFFYBattleCharacter* GetBattleCharacterReference()
	{
		return BattleCharacterReference;
	};
	
	virtual void OnSelected() override;
	virtual void OnUnselected() override;
	virtual void PerformAction() override;
	
};
