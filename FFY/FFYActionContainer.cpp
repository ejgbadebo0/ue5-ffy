// Source code implementation by Ephraim Gbadebo.


#include "FFYActionContainer.h"

#include "Actions/FFYItemAction.h"

void UFFYActionContainer::AddAction(AFFYAction* Action)
{
	if (Action)
	{
		if (Action->bIsPassive)
		{
			Passives.Emplace(Action);
		}
		else
		{
			switch (Action->ActionType)
			{
				default:
					MainCommands.Emplace(Action);
					break;
				case EActionType::MAGIC:
					Magic.Emplace(Action);
					break;
				case EActionType::ITEM:
					if (Action->Label == FName("Item"))
					{
						Item = Cast<AFFYItemAction>(Action);
						Item->OnItemActionUse.AddUniqueDynamic(this, &UFFYActionContainer::ItemUseEvent);
					}
			}
		}
	}
	
}

//Callback event for item use in battles
void UFFYActionContainer::ItemUseEvent(FName ID, AFFYBattleCharacter* User, AFFYBattleCharacter* Target)
{
	OnItemUse.Broadcast(ID, User, Target);
}
