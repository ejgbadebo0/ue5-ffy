// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFYDataEnums.h"
#include "UObject/Interface.h"
#include "FFYWidgetEvents.generated.h"

struct FDamageEventResult;
class AFFYBattleCharacter;
enum class EEquipmentSlot : uint8;
enum class EEquipmentClass : uint8;
class UFFYEquipmentItemOptionWidget;
class UFFYPartyMemberOptionWidget;
class UFFYOptionWidget;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFFYWidgetEvents : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FFY_API IFFYWidgetEvents
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ViewportResized(int ViewportX, int ViewportY);

	//Unselect all options
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ResetOptions();

	//update playtime through paused widgets
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdatePlayTime();

	//Called up by OptionWidget to PartySelectionMenu
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PartyMemberSelect(UFFYPartyMemberOptionWidget* CharacterWidget);
	
	//StartSelection
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartSelection(UFFYOptionWidget* SelectedOption, ETargetType SelectedTargetType = ETargetType::SINGLE);

	//EndSelection
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EndSelection();

	//execute logic based on exit conditions
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EndConfirmation(bool bIsConfirmed, bool bShouldExitMenu);

	//Called on menu from button press
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartConfirmation(FName CallbackName);

	//Called on Confirm menu widgets
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetConfirmMenuText(const FText& Text);

	//called on GameInstance to get function name on unpause
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FName GetConfirmCallbackName();

	//Custom functionality based on a Selected party member button pressed
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ContextAction(UFFYPartyMemberOptionWidget* CharacterWidget, bool SelectAll);

	//Custom functionality based on Selected BattleCharacter
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BattleContextAction(AFFYBattleCharacter* Character, bool SelectAll);
	
	//From menu navigation function, for menus that require party member info
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void LoadContext(FName ContextName);

	//Version to use in battle widget
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void LoadBattleContext(AFFYBattleCharacter* Character);

	//Use in battle widget to get user of actions
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AFFYBattleCharacter* GetBattleContext();

	//Use from buttons/menus/player
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetBattleContext(AFFYBattleCharacter* Character);
	
	//Custom selective refresh function for current context.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ContextRefresh(int Index);

	//Return if menu has SelectAll enabled.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetSelectAll();

	//Tell equipment menu to load equipment items based on parameters
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void LoadEquipmentItems(EEquipmentClass EquipmentClass, EEquipmentSlot EquipmentSlot);

	//EquipmentItemOptionWidget custom button events
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EquipmentItemSelected(UFFYEquipmentItemOptionWidget* SelectedItemWidget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EquipmentItemHovered(UFFYEquipmentItemOptionWidget* SelectedItemWidget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EquipmentItemUnhovered(UFFYEquipmentItemOptionWidget* SelectedItemWidget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SelectAllInputEvent();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SwitchTargetsInputEvent();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CycleInputEvent(int DeltaIndex);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DefendInputEvent();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ContextCommandInputEvent(int DeltaIndex);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetInputKeyText();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetDefaultTargetGroup(bool bIsEnemy, bool Reset);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetDefaultTargetGroup();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<AFFYBattleCharacter*> GetTargetGroupActors(bool bIsEnemies);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MenuActionSelected(AFFYAction* SelectedAction);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayMenuSound(uint8 SoundIndex);
	
};
