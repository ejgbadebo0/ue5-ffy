// Source code implementation by Ephraim Gbadebo.
#pragma once

#include "CoreMinimal.h"
#include "FFY/FFYWidgetEvents.h"
#include "FFY/Widgets/FFYOptionWidget.h"
#include "FFYActionOptionWidget.generated.h"

class AFFYAction;
/**
 * 
 */
UCLASS()
class FFY_API UFFYActionOptionWidget : public UFFYOptionWidget, public IFFYWidgetEvents
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;

	int UserIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	UTextBlock* ActionNameBlock;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	UTextBlock* MPCostBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	UTextBlock* HPCostBlock;

public:

	void InitializeOption(AFFYAction* ActionRef, int UserIndex);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Action)
	AFFYAction* Action;
	
	virtual void SetIsDisabled(bool Value) override;
	virtual FText GetDescription() override;
	virtual void OnSelected() override;
	virtual void OnUnselected() override;
	virtual void PerformAction() override;

	virtual void ContextAction_Implementation(UFFYPartyMemberOptionWidget* CharacterWidget, bool SelectAll) override;
};
