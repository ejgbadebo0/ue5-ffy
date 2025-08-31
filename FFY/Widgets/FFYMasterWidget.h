// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetSwitcher.h"
#include "FFY/FFYWidgetEvents.h"
#include "FFYMasterWidget.generated.h"

class URichTextBlock;
class UFFYSelectPartyMemberWidget;
enum class EMenuMode : uint8;
class UFFYMenuWidget;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMenuChanged);

UCLASS(Abstract)
class FFY_API UFFYMasterWidget : public UUserWidget, public IFFYWidgetEvents
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	URichTextBlock* InputKeyLegend;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	UFFYSelectPartyMemberWidget* SelectionWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TMap<FName, int> Menus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TMap<FName, FName> PreviousMenus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	FName CurrentMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	EMenuMode CurrentMenuMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	int TimeInMenu = 0;

public:
	//DELEGATE:
	UPROPERTY(BlueprintAssignable)
	FOnMenuChanged OnMenuChanged;
	//--------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	bool bShouldKeepTime = false;
	
	virtual void NavigateToMenu(FName Menu, FName ContextName);

	virtual void PreviousMenu();

	UWidgetSwitcher* GetWidgetSwitcher() { return WidgetSwitcher; }

	EMenuMode GetCurrentMenuMode() { return CurrentMenuMode; };

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UFFYSelectPartyMemberWidget* GetMasterSelectionWidget();

	void SetMenuMode(EMenuMode Mode) { CurrentMenuMode = Mode; };

	//INTERFACE:
	virtual void EndSelection_Implementation() override;
};
