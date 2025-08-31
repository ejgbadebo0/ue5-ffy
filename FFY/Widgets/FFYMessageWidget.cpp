// Source code implementation by Ephraim Gbadebo.


#include "FFYMessageWidget.h"

#include "Components/RichTextBlock.h"
#include "FFY/FFYCharacter.h"
#include "Kismet/GameplayStatics.h"

void UFFYMessageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (MessageTextBlock && !Messages.IsEmpty())
	{
		MessageTextBlock->SetText(Messages[CurrentMessageIndex]);
	}
	PlayerCharacter = Cast<AFFYCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "PlayerCharacter found!");
		PlayerCharacter->CurrentMessageWidget = this;
		PlayerCharacter->CharacterState = ECharacterState::MESSAGE;
	}
}

void UFFYMessageWidget::AdvanceMessage()
{

	CurrentMessageIndex++;
	
	if (MessageTextBlock)
	{
		if (CurrentMessageIndex <= Messages.Num() - 1)
		{
			MessageTextBlock->SetText(Messages[CurrentMessageIndex]);
		}
		else
		{
			if (PlayerCharacter)
			{
				PlayerCharacter->CurrentMessageWidget = nullptr;
				PlayerCharacter->CharacterState = ECharacterState::NONE;
				this->RemoveFromParent();
			}
		}
	}
}
