// Source code implementation by Ephraim Gbadebo.


#include "FFYItem.h"

#include "FFYBattleCharacter.h"
#include "FFYGameInstance.h"

void UFFYItem::DecrementItem(FItemData& Item)
{
	UFFYGameInstance* GameInstance = Cast<UFFYGameInstance>(GetWorld()->GetGameInstance());
	
	Item.Amount -= 1;
	if (Item.Amount <= 0)
	{
		if (GameInstance)
		{
			GameInstance->RemoveFromInventory(Item);
		}
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("BROADCAST: OnInventoryUpdated."));
		GameInstance->OnInventoryUpdated.Broadcast();
	}
}

void UFFYItem::ModifyStats(FItemData& Item, FBattleCharacterData& Target, float Value)
{
	if (Item.ID != NAME_None) 
	{
		Target.Strength += Item.EquipmentData.Strength * Value;
		Target.Constitution += Item.EquipmentData.Constitution * Value;
		Target.Magic += Item.EquipmentData.Magic * Value;
		Target.Spirit += Item.EquipmentData.Spirit * Value;
		Target.Precision += Item.EquipmentData.Precision * Value;
		Target.Dexterity += Item.EquipmentData.Dexterity * Value;
		Target.Luck += Item.EquipmentData.Luck * Value;

		if (Value > 0) //Add when 1
		{
			if (!Item.EquipmentData.Weaknesses.IsEmpty())
			{
				for (EElement i : Item.EquipmentData.Weaknesses)
				{
					Target.Weaknesses.Add(i);
				}
			}
			if (!Item.EquipmentData.Resistances.IsEmpty())
			{
				for (EElement i : Item.EquipmentData.Resistances)
				{
					Target.Resistances.Add(i);
				}
			}
			if (!Item.EquipmentData.Catalysts.IsEmpty())
			{
				for (EElement i : Item.EquipmentData.Catalysts)
				{
					Target.Catalysts.Add(i);
				}
			}
			if (!Item.EquipmentData.Immunities.IsEmpty())
			{
				for (EStatusEffect i : Item.EquipmentData.Immunities)
				{
					Target.Immunities.Add(i);
				}
			}
		}
		
		else //Remove when -1
		{
			if (!Item.EquipmentData.Weaknesses.IsEmpty())
			{
				for (EElement i : Item.EquipmentData.Weaknesses)
				{
					Target.Weaknesses.RemoveSingle(i);
				}
			}
			if (!Item.EquipmentData.Resistances.IsEmpty())
			{
				for (EElement i : Item.EquipmentData.Resistances)
				{
					Target.Resistances.RemoveSingle(i);
				}
			}
			if (!Item.EquipmentData.Catalysts.IsEmpty())
			{
				for (EElement i : Item.EquipmentData.Catalysts)
				{
					Target.Catalysts.RemoveSingle(i);
				}
			}
			if (!Item.EquipmentData.Immunities.IsEmpty())
			{
				for (EStatusEffect i : Item.EquipmentData.Immunities)
				{
					Target.Immunities.RemoveSingle(i);
				}
			}
			
		}
		
	}
}

void UFFYItem::Use(FItemData& Item, FBattleCharacterData& Target)
{

	EItemFuncCode Code = Item.ItemFuncCode;
	
	FItemUseResult Result = ItemEffect(Code, Target);
	if (Result.bWasUsed)
	{
		DecrementItem(Item);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("ItemUse failed..."));
	}
	
}

void UFFYItem::BattleUse(FItemData& Item, AFFYBattleCharacter* User, AFFYBattleCharacter* Target)
{
	EItemFuncCode Code = Item.ItemFuncCode;
	
	FItemUseResult Result = ItemEffect(Code, Target->BattleCharacterStats);
	//decrement even if target doesn't receive the effect
	DecrementItem(Item);
	//call event to display result
	Target->OnCharacterStatsChanged.Broadcast(Target->BattleCharacterStats);
	Target->DamageTakenEvent(FDamageEventResult(Result.bWasUsed, false, true, false, Result.Amount, Result.ResultDescription));

}

FItemUseResult UFFYItem::ItemEffect(EItemFuncCode ItemFuncCode, FBattleCharacterData& Target)
{
	FItemUseResult ItemUseResult = FItemUseResult();
	bool Result = false;
	
	switch (ItemFuncCode)
	{
		default:
			Result = false;
			break;

		case EItemFuncCode::POTION:
			float RecoverValue = 50.f;
			ItemUseResult.Amount = RecoverValue;
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("POTION- HP: %f, MAXHP: %f"), Target.HP, Target.MaxHP));

			Result = (Target.HP < Target.MaxHP && !Target.StatusEffects.Contains(EStatusEffect::KO));
			if (Result)
			{
				Target.HP = FMath::Min(Target.HP + RecoverValue, Target.MaxHP);
			}
			break;
			
	}
	ItemUseResult.bWasUsed = Result;
	return ItemUseResult;
}

void UFFYItem::Equip(FItemData& Item, FBattleCharacterData& Target)
{
	if (Item.bIsEquipable)
	{
		switch (Item.EquipmentData.EquipmentSlot)
		{
			default:
				break;
			case EEquipmentSlot::NONE:
				break;
			//WEAPON
			case EEquipmentSlot::WEAPON:
				if (Target.Equipment.Weapon.ID != NAME_None)
				{
					Unequip(Target.Equipment.Weapon, Target);
				}
				Target.Equipment.Weapon = Item;
				ModifyStats(Item, Target);
				DecrementItem(Item); 
				break;
			//SHIELD
			case EEquipmentSlot::SHIELD:
				if (Target.Equipment.Shield.ID != NAME_None)
				{
					Unequip(Target.Equipment.Shield, Target);
				}
				Target.Equipment.Shield = Item;
				ModifyStats(Item, Target);
				DecrementItem(Item);
				break;
			//ARMOR
			case EEquipmentSlot::ARMOR:
				if (Target.Equipment.Armor.ID != NAME_None)
				{
					Unequip(Target.Equipment.Armor, Target);
				}
				Target.Equipment.Armor = Item;
				ModifyStats(Item, Target);
				DecrementItem(Item); 
				break;
			//ACCESSORIES
			case EEquipmentSlot::ACCESSORY:
				if (Target.Equipment.Accessory.ID != NAME_None)
				{
					Unequip(Target.Equipment.Accessory, Target);
				}
				Target.Equipment.Accessory = Item;
				ModifyStats(Item, Target);
				DecrementItem(Item);
				break;
		}
	}
}

void UFFYItem::Unequip(FItemData& Item, FBattleCharacterData& Target)
{
	UFFYGameInstance* GameInstance = Cast<UFFYGameInstance>(GetWorld()->GetGameInstance());
	//Make temp items to ensure proper inventory updates
	FItemData ItemCopy = Item;
	FItemData NullItem = FItemData();
	
	switch (Item.EquipmentData.EquipmentSlot)
	{
		default:
			break;
		case EEquipmentSlot::NONE:
			break;
		case EEquipmentSlot::WEAPON:
			if (Item.ID != NAME_None)
			{
				ModifyStats(Item, Target, -1.f);
				Target.Equipment.Weapon = NullItem;
			}
			break;
		case EEquipmentSlot::SHIELD:
			if (Item.ID != NAME_None)
			{
				ModifyStats(Item, Target, -1.f);
				Target.Equipment.Shield = NullItem;
			}
			break;
		case EEquipmentSlot::ARMOR:
			if (Item.ID != NAME_None)
			{
				ModifyStats(Item, Target, -1.f);
				Target.Equipment.Armor = NullItem;
			}
			break;
		case EEquipmentSlot::ACCESSORY:
			if (Item.ID != NAME_None)
			{
				ModifyStats(Item, Target, -1.f);
				Target.Equipment.Accessory = NullItem;
			}
			break;
	}
	GameInstance->AddToInventory(ItemCopy, 1);
}

void UFFYItem::Unequip(EEquipmentSlot Slot, FBattleCharacterData& Target)
{
	switch (Slot)
	{
		default:
			break;
		case EEquipmentSlot::NONE:
			break;
		case EEquipmentSlot::WEAPON:
			Unequip(Target.Equipment.Weapon, Target);
			break;
		case EEquipmentSlot::SHIELD:
			Unequip(Target.Equipment.Shield, Target);
			break;
		case EEquipmentSlot::ARMOR:
			Unequip(Target.Equipment.Armor, Target);
			break;
		case EEquipmentSlot::ACCESSORY:
			Unequip(Target.Equipment.Accessory, Target);
			break;
	}
}

/*
 * Compare Equipment item to Target characters current item in that slot.
 */
FEquipmentData UFFYItem::Compare(FItemData& Item, FBattleCharacterData& Target)
{
	EEquipmentSlot CompareSlot = Item.EquipmentData.EquipmentSlot;
	FEquipmentData CompareData = FEquipmentData();

	bool Result = false;

	switch (CompareSlot)
	{
	default:
		return FEquipmentData();
	case EEquipmentSlot::NONE:
		return FEquipmentData();
	case EEquipmentSlot::WEAPON:
		if (Target.Equipment.Weapon.ID != NAME_None)
		{
			CompareData.EquipmentSlot = CompareSlot;
			CompareData.Strength = Target.Strength - Target.Equipment.Weapon.EquipmentData.Strength + Item.EquipmentData.Strength;
			CompareData.Constitution = Target.Constitution - Target.Equipment.Weapon.EquipmentData.Constitution + Item.EquipmentData.Constitution;
			CompareData.Magic = Target.Magic - Target.Equipment.Weapon.EquipmentData.Magic + Item.EquipmentData.Magic;
			CompareData.Spirit = Target.Spirit - Target.Equipment.Weapon.EquipmentData.Spirit + Item.EquipmentData.Spirit;
			CompareData.Precision = Target.Precision - Target.Equipment.Weapon.EquipmentData.Precision + Item.EquipmentData.Precision;
			CompareData.Dexterity = Target.Dexterity - Target.Equipment.Weapon.EquipmentData.Dexterity + Item.EquipmentData.Dexterity;
			CompareData.Luck = Target.Luck - Target.Equipment.Weapon.EquipmentData.Luck + Item.EquipmentData.Luck;
			Result = true;
		}
		break;
		
		case EEquipmentSlot::SHIELD:
			if (Target.Equipment.Shield.ID != NAME_None)
			{
				CompareData.EquipmentSlot = CompareSlot;
				CompareData.Strength = Target.Strength - Target.Equipment.Shield.EquipmentData.Strength + Item.EquipmentData.Strength;
				CompareData.Constitution = Target.Constitution - Target.Equipment.Shield.EquipmentData.Constitution + Item.EquipmentData.Constitution;
				CompareData.Magic = Target.Magic - Target.Equipment.Shield.EquipmentData.Magic + Item.EquipmentData.Magic;
				CompareData.Spirit = Target.Spirit - Target.Equipment.Shield.EquipmentData.Spirit + Item.EquipmentData.Spirit;
				CompareData.Precision = Target.Precision - Target.Equipment.Shield.EquipmentData.Precision + Item.EquipmentData.Precision;
				CompareData.Dexterity = Target.Dexterity - Target.Equipment.Shield.EquipmentData.Dexterity + Item.EquipmentData.Dexterity;
				CompareData.Luck = Target.Luck - Target.Equipment.Shield.EquipmentData.Luck + Item.EquipmentData.Luck;
				Result = true;
			}
			break;
		
		case EEquipmentSlot::ARMOR:
			if (Target.Equipment.Armor.ID != NAME_None)
			{
				CompareData.EquipmentSlot = CompareSlot;
				CompareData.Strength = Target.Strength - Target.Equipment.Armor.EquipmentData.Strength + Item.EquipmentData.Strength;
				CompareData.Constitution = Target.Constitution - Target.Equipment.Armor.EquipmentData.Constitution + Item.EquipmentData.Constitution;
				CompareData.Magic = Target.Magic - Target.Equipment.Armor.EquipmentData.Magic + Item.EquipmentData.Magic;
				CompareData.Spirit = Target.Spirit - Target.Equipment.Armor.EquipmentData.Spirit + Item.EquipmentData.Spirit;
				CompareData.Precision = Target.Precision - Target.Equipment.Armor.EquipmentData.Precision + Item.EquipmentData.Precision;
				CompareData.Dexterity = Target.Dexterity - Target.Equipment.Armor.EquipmentData.Dexterity + Item.EquipmentData.Dexterity;
				CompareData.Luck = Target.Luck - Target.Equipment.Armor.EquipmentData.Luck + Item.EquipmentData.Luck;
				Result = true;
			}
			break;

	case EEquipmentSlot::ACCESSORY:
		if (Target.Equipment.Accessory.ID != NAME_None)
		{
			CompareData.EquipmentSlot = CompareSlot;
			CompareData.Strength = Target.Strength - Target.Equipment.Accessory.EquipmentData.Strength + Item.EquipmentData.Strength;
			CompareData.Constitution = Target.Constitution - Target.Equipment.Accessory.EquipmentData.Constitution + Item.EquipmentData.Constitution;
			CompareData.Magic = Target.Magic - Target.Equipment.Accessory.EquipmentData.Magic + Item.EquipmentData.Magic;
			CompareData.Spirit = Target.Spirit - Target.Equipment.Accessory.EquipmentData.Spirit + Item.EquipmentData.Spirit;
			CompareData.Precision = Target.Precision - Target.Equipment.Accessory.EquipmentData.Precision + Item.EquipmentData.Precision;
			CompareData.Dexterity = Target.Dexterity - Target.Equipment.Accessory.EquipmentData.Dexterity + Item.EquipmentData.Dexterity;
			CompareData.Luck = Target.Luck - Target.Equipment.Accessory.EquipmentData.Luck + Item.EquipmentData.Luck;
			Result = true;
		}
		break;
			
	}
	
	if (!Result)
	{
		CompareData.EquipmentSlot = CompareSlot;
		CompareData.Strength = Target.Strength + Item.EquipmentData.Strength;
		CompareData.Constitution = Target.Constitution + Item.EquipmentData.Constitution;
		CompareData.Magic = Target.Magic + Item.EquipmentData.Magic;
		CompareData.Spirit = Target.Spirit + Item.EquipmentData.Spirit;
		CompareData.Precision = Target.Precision + Item.EquipmentData.Precision;
		CompareData.Dexterity = Target.Dexterity + Item.EquipmentData.Dexterity;
		CompareData.Luck = Target.Luck + Item.EquipmentData.Luck;
	}
	
	return CompareData;
}

