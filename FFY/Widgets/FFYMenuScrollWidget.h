// Source code implementation by Ephraim Gbadebo.
#pragma once

#include "CoreMinimal.h"
#include "FFY/Widgets/FFYMenuWidget.h"
#include "FFYMenuScrollWidget.generated.h"

class UScrollBox;
/**
 * 
 */
UCLASS()
class FFY_API UFFYMenuScrollWidget : public UFFYMenuWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	UScrollBox* ScrollBox;

	virtual void ClearOptions() override;
	
	virtual void Refresh() override;

public:
	virtual void SetDefaultTargetGroup_Implementation(bool bIsEnemy, bool Reset) override
	{
		Super::SetDefaultTargetGroup_Implementation(bIsEnemy, Reset);
	}
};
