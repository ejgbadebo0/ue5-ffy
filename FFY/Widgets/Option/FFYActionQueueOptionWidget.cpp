// Source code implementation by Ephraim Gbadebo.


#include "FFYActionQueueOptionWidget.h"

#include "Components/Image.h"
#include "FFY/FFYBattleCharacter.h"


void UFFYActionQueueOptionWidget::ExecuteAction(float Value)
{
	if (Value >= TargetATB)
	{
		if (QueueAction)
		{
			OnPressed();
			CharacterReference->SetActionState_Implementation(EActionState::IDLE, false);
			QueueAction->ExecuteAction(CharacterReference, QueueTargets);
		}
		Unload();
	}
}

void UFFYActionQueueOptionWidget::ExecuteActionFromState(EActionState NewState)
{
	if (NewState == EActionState::IDLE)
	{
		CharacterReference->OnActionStateChanged.RemoveDynamic(this, &UFFYActionQueueOptionWidget::ExecuteActionFromState);
		CharacterReference->OnATBValueChanged.AddUniqueDynamic(this, &UFFYActionQueueOptionWidget::ExecuteAction);
		CharacterReference->SetActionState_Implementation(EActionState::CASTING, false);
	}
}

void UFFYActionQueueOptionWidget::ExecuteActionFromWait(EActiveState NewActiveState)
{
	if (NewActiveState == EActiveState::ACTIVE)
	{
		CharacterReference->OnActiveStateChanged.RemoveDynamic(this, &UFFYActionQueueOptionWidget::ExecuteActionFromWait);
		if (CharacterReference->ActionState != EActionState::ACTING)
		{
			ExecuteAction(CharacterReference->ATB);
		}
		
		//ExecuteActionFromState(CharacterReference->ActionState); //need to check if was acting before being set to wait
	}
}

void UFFYActionQueueOptionWidget::Queue(AFFYAction* Action, AFFYBattleCharacter* Character,
                                        TArray<AFFYBattleCharacter*> Targets)
{
	if (Character && Portrait)
	{
		CharacterReference = Character;
		QueueTargets = Targets;
		QueueAction = Action;

		TargetATB = QueueAction->ATBCost;

		//check queue conditions for character
		if (CharacterReference->ActiveState == EActiveState::WAIT) //is in Wait mode due to another characters action
		{
			CharacterReference->OnActiveStateChanged.AddUniqueDynamic(this, &UFFYActionQueueOptionWidget::ExecuteActionFromWait);
		}
		if (CharacterReference->ActionState == EActionState::ACTING) //performing another action already
		{
			CharacterReference->OnActionStateChanged.AddUniqueDynamic(this, &UFFYActionQueueOptionWidget::ExecuteActionFromState);
		}
		else  //ATB resource cases
		{
			CharacterReference->OnATBValueChanged.AddUniqueDynamic(this, &UFFYActionQueueOptionWidget::ExecuteAction);
			CharacterReference->SetActionState_Implementation(EActionState::CASTING, false);
		}


		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Black, TEXT("QUEUEING MOVE"));
		UMaterialInstanceDynamic* MaterialInstance = Portrait->GetDynamicMaterial();
		if (MaterialInstance)
		{
			MaterialInstance->SetTextureParameterValue("InTexture", CharacterReference->PortraitImage);
		}
		
		if (Character && CommandTextBlock)
		{
			CommandTextBlock->SetText(FText::FromName(Action->GetMenuLabel()));
		}
		SetIsDisabled(false);
		//BP functions
		OnReload();
	}
}

void UFFYActionQueueOptionWidget::Unload()
{
	if (CharacterReference)
	{
		if (CharacterReference->ActionState == EActionState::CASTING) //final state check
		{
			CharacterReference->SetActionState_Implementation(EActionState::IDLE, false);
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Black, TEXT("QUEUE: SETTING IDLE"));
		}
		if (CharacterReference->ActiveState == EActiveState::WAIT) //is in Wait mode due to another characters action
		{
			CharacterReference->OnActiveStateChanged.RemoveDynamic(this, &UFFYActionQueueOptionWidget::ExecuteActionFromWait);
		}
		CharacterReference->OnATBValueChanged.RemoveDynamic(this, &UFFYActionQueueOptionWidget::ExecuteAction);
		CharacterReference = nullptr;

		OnDequeue.Broadcast();
		Super::Unload();
	}
}

void UFFYActionQueueOptionWidget::PerformAction()
{
	Unload();
}
