// Source code implementation by Ephraim Gbadebo.


#include "FFYContextCommandOptionWidget.h"

#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "FFY/FFYBattleCharacter.h"

void UFFYContextCommandOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (InputTextBlock)
	{
		InputTextBlock->SetText(DefaultInputText);
	}
	SetVisibility(ESlateVisibility::Hidden);
}

void UFFYContextCommandOptionWidget::Reload(AFFYBattleCharacter* Character)
{
	if (Character && Portrait)
	{
		CharacterReference = Character;
		UMaterialInstanceDynamic* MaterialInstance = Portrait->GetDynamicMaterial();
		if (MaterialInstance)
		{
			MaterialInstance->SetTextureParameterValue("InTexture", CharacterReference->BattleCharacterStats.Portrait);
			MaterialInstance->SetScalarParameterValue("Offset", CharacterReference->PortraitOffset);
		}
		
		if (Character->ActionContainer->ContextCommand && CommandTextBlock)
		{
			CommandTextBlock->SetText(FText::FromName(Character->ActionContainer->ContextCommand->Label));
		}
		SetIsDisabled(false);
		//BP functions
		OnReload();
	}
	
}

void UFFYContextCommandOptionWidget::Unload()
{
	SetIsDisabled(true);
	CharacterReference = nullptr;
	OnUnload();
}

void UFFYContextCommandOptionWidget::PerformAction()
{
	if (bIsDisabled)
	{
		return;
	}

	OnPressed();
	if (CharacterReference)
	{
		if (CharacterReference->ActionContainer->ContextCommand)
		{
			CharacterReference->ActionContainer->ContextCommand->ExecuteAction(CharacterReference, CharacterReference->Targets);
			Unload();
		}
	}
}
