// Fill out your copyright notice in the Description page of Project Settings.

#include "FFY/FFYBattleCharacter.h"

#include "FFYActionQueueMenuWidget.h"

#include "Components/WrapBox.h"
#include "FFY/Widgets/Option/FFYActionQueueOptionWidget.h"

void UFFYActionQueueMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	for (int i = 0; i < 3; i++)
	{
		UFFYActionQueueOptionWidget* ToAdd = CreateWidget<UFFYActionQueueOptionWidget>(this, QueueOptionsClass);
		if (ToAdd)
		{
			QueueOptions.Emplace(ToAdd);
			ToAdd->SetOwnerMenu(this);
		}
	}
	//first on bottom
	for (int i = QueueOptions.Num()-1; i >= 0; i--)
	{
		QueueOptions[i]->OnDequeue.AddUniqueDynamic(this, &UFFYActionQueueMenuWidget::OnDequeue);
		if (WrapBox)
		{
			WrapBox->AddChildToWrapBox(QueueOptions[i]);
		}
	}
}

void UFFYActionQueueMenuWidget::InitializeParty(TArray<AFFYBattleCharacter*> Party)
{
	for (auto p : Party)
	{
		if (p)
		{
			p->OnActionQueued.AddUniqueDynamic(this, &UFFYActionQueueMenuWidget::AddToQueue);
		}
	}
}

void UFFYActionQueueMenuWidget::AddToQueue(AFFYAction* Action, AFFYBattleCharacter* User,
	TArray<AFFYBattleCharacter*> ActionTargets)
{
	if (NumQueued == 0)
	{
		OnQueue();
	}

	if (Action && ActionTargets.Num() > 0 && User)
	{
		for (int i = 0; i < QueueOptions.Num() - 1; i++)
		{
			if (!QueueOptions[i]->CharacterReference)
			{
				QueueOptions[i]->Queue(Action, User, ActionTargets);
				NumQueued++;
				break;
			}
		}
	}
}

void UFFYActionQueueMenuWidget::OnDequeue()
{
	NumQueued--;
	if (NumQueued <= 0)
	{
		NumQueued = 0;
		OnEmpty();
	}
}
