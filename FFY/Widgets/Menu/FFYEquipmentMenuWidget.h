// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Widgets/FFYMenuScrollWidget.h"
#include "FFYEquipmentMenuWidget.generated.h"

class UFFYSlotEquipOptionWidget;
class UFFYEquipmentItemOptionWidget;
class UScrollBox;
class URichTextBlock;
/**
 * 
 */
UCLASS()
class FFY_API UFFYEquipmentMenuWidget : public UFFYMenuScrollWidget
{
	
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
    
    	//Initialized through LoadContext and use to navigate upon Cycle input. 

	//Default Widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UFFYSlotEquipOptionWidget* WeaponSlotWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UFFYSlotEquipOptionWidget* ShieldSlotWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UFFYSlotEquipOptionWidget* ArmorSlotWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UFFYSlotEquipOptionWidget* AccessorySlotWidget;

	
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
	URichTextBlock* MaxHPValue;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	URichTextBlock* MaxMPValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* EXPValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* ReqEXPValue;
	
    
	//Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* StrengthValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* DeltaSTR;
    	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* ConstitutionValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* DeltaCON;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* MagicValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* DeltaMAG;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* SpiritValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* DeltaSPR;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* PrecisionValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* DeltaPRS;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* DexterityValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* DeltaDEX;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* LuckValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* DeltaLUC;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Widget)
	TSubclassOf<UFFYEquipmentItemOptionWidget> ItemButtonClass;
	
	virtual void Refresh() override;

	void ClearDeltaValues() const;

	void ConstructUnequipOption(EEquipmentClass EquipmentClass, EEquipmentSlot EquipmentSlot);

	//INTERFACE:
	virtual void EndSelection_Implementation() override;
	virtual void StartSelection_Implementation(UFFYOptionWidget* SelectedOption, ETargetType SelectedTargetType) override;
	virtual void LoadContext_Implementation(FName ContextName) override;
	virtual FText GetInputKeyText_Implementation() override
	{
		return FText::FromString("<img id=\"LeftMouseButton\"></> = Confirm <img id=\"RightMouseButton\"></> = Cancel <img id=\"Q\"></>/<img id=\"E\"></> = Cycle Members");
	}


	virtual void EquipmentItemHovered_Implementation(UFFYEquipmentItemOptionWidget* SelectedItemWidget) override;
	virtual void EquipmentItemSelected_Implementation(UFFYEquipmentItemOptionWidget* SelectedItemWidget) override;
	virtual void EquipmentItemUnhovered_Implementation(UFFYEquipmentItemOptionWidget* SelectedItemWidget) override;
	virtual void LoadEquipmentItems_Implementation(EEquipmentClass EquipmentClass, EEquipmentSlot EquipmentSlot) override;
};



