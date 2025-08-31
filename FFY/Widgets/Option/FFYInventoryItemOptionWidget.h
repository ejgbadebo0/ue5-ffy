// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/FFYItem.h"
#include "FFY/FFYWidgetEvents.h"
#include "FFY/Widgets/FFYOptionWidget.h"
#include "FFYInventoryItemOptionWidget.generated.h"

class UFFYItem;
/**
 * 
 */
UCLASS()
class FFY_API UFFYInventoryItemOptionWidget : public UFFYOptionWidget, public IFFYWidgetEvents
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual FText GetDescription() override
	{
		return  (InventoryItem.ID.IsValid()) ? InventoryItem.Description : FText::GetEmpty();
	}

public:
	virtual void OnSelected() override;
	virtual void OnUnselected() override;
	virtual void PerformAction() override;
	virtual void SetIsDisabled(bool Value) override;
	void InitializeOption(const FItemData& InventoryItemReference);
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Inventory)
	FItemData InventoryItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Inventory, meta = (BindWidget))
	UTextBlock* InventoryItemNameBlock;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Inventory, meta = (BindWidget))
	UTextBlock* InventoryItemAmountBlock;

	virtual void ContextAction_Implementation(UFFYPartyMemberOptionWidget* CharacterWidget, bool SelectAll) override;
};


