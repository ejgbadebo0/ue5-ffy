// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFY/Widgets/FFYMenuWidget.h"
#include "FFYBattleActionMenuWidget.generated.h"

/**
 * 
 */

class UFFYBattleActionOptionWidget;

UENUM(BlueprintType)
enum class EActionMenuType : uint8
{
	NONE UMETA(DisplayName = "None"),
	MAGIC UMETA(DisplayName = "Magic"),
	SKILL UMETA(DisplayName = "Skill"),
	E_MAX UMETA(Hidden)
};

UCLASS()
class FFY_API UFFYBattleActionMenuWidget : public UFFYMenuWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UFFYBattleActionOptionWidget> ActionOptionClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	EActionMenuType ActionMenuType = EActionMenuType::MAGIC;

	UFUNCTION(BlueprintCallable, Category = UI)
	EActionMenuType GetActionMenuType()
	{
		return ActionMenuType;
	};

	//INTERFACE:
	virtual void LoadBattleContext_Implementation(AFFYBattleCharacter* Character) override;
	virtual void SwitchTargetsInputEvent_Implementation() override
	{
		Super::SwitchTargetsInputEvent_Implementation();
	}
	virtual UFFYSelectPartyMemberWidget* GetSelectionWidget_Implementation() override;
	virtual FText GetInputKeyText_Implementation() override
	{
		return FText::FromString("<img id=\"LeftMouseButton\"></> = Confirm\n \n <img id=\"RightMouseButton\"></> = Cancel\n \n <img id=\"C\"></> = Select All\n \n <img id=\"X\"></> = Switch Target Group\n \n");
	}
};
