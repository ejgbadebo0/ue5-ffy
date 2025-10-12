// Source code implementation by Ephraim Gbadebo.


#include "FFYGambitComponent.h"

#include "FFY/FFYBattleCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UFFYGambitComponent::UFFYGambitComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = bGambitsActive;

	// ...
}


// Called when the game starts
void UFFYGambitComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	OwnerCharacter = Cast<AFFYBattleCharacter>(GetOwner());
	if (OwnerCharacter)
	{
		OwnerCharacter->OnATBValueChanged.AddUniqueDynamic(this, &UFFYGambitComponent::EvaluateGambits);
	}
	
}


// Called every frame
void UFFYGambitComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

/*
 * Check all gambits and evaluate if any actions can be performed.
 * If more than one gambit returns true, sort based on priority and randomly select
 * from the highest priority gambits.
 */
void UFFYGambitComponent::EvaluateGambits(float ATBValue) 
{
	if (ATBValue >= 100.f)
	{
		//Iterate over Action/Gambit entries to see if any can be used
		
		if (!Gambits.IsEmpty())
		{
			for (int i = 0; i < Gambits.Num(); i++)
			{
				//1: determine if gambit can be evaluated
				if (Gambits[i].MaxCooldown >= 0) 
				{
					if (Gambits[i].EvaluateCooldown > 0)
					{
						Gambits[i].EvaluateCooldown--;
					}
					else
					{
						//2: find targets for gambit action ===========================
						TArray<AFFYBattleCharacter*> Targets;
						bool bTargetAll; 
						switch (Gambits[i].TargetType)
						{
							default:
								bTargetAll = false;
								break;
							case ETargetType::SINGLE:
								bTargetAll = false;
								break;
							case ETargetType::MULTI:
								bTargetAll = true;
								break;
							case ETargetType::BOTH:
								bTargetAll = FMath::RandBool();
						}

						if (Gambits[i].TargetClass == ETargetClass::SELF)
						{
							if (ValidGambitConditions(OwnerCharacter, Gambits[i]))
							{
								Targets.Emplace(OwnerCharacter);
							}
						}
						else
						{
							TArray<AActor*> OutActors;

							switch (Gambits[i].TargetClass)
							{
							case ETargetClass::NONE:
								UGameplayStatics::GetAllActorsOfClass(GetWorld(), TSubclassOf<AFFYBattleCharacter>(), OutActors);
								break;
							case ETargetClass::PARTY:
								UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Party"), OutActors);
								break;
							case ETargetClass::ENEMY:
								UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Enemy"), OutActors);
								break;
							case ETargetClass::RANDOM:
								UGameplayStatics::GetAllActorsOfClass(GetWorld(), TSubclassOf<AFFYBattleCharacter>(), OutActors);
								break;
							}
							//3: Evaluate Gambit conditions: 
							for (AActor* Actor : OutActors)
							{
								AFFYBattleCharacter* Character = Cast<AFFYBattleCharacter>(Actor);
								if (Character)
								{
									//check if we want KO'd characters to be selectable and evaluate each member to see if they meet gambit conditions
									if ( (!Gambits[i].bFilterDead || (Gambits[i].bFilterDead && !Character->BattleCharacterStats.StatusEffects.Contains(EStatusEffect::KO))) && ValidGambitConditions(Character, Gambits[i]) )
									{
										Targets.Emplace(Character);
									}
								}
							}
						}

						//4: Select a valid target
						bool Result = false;
						//resize array if not targeting all entities
						if (Targets.Num() > 0)
						{
							if (!bTargetAll)
							{
								int TargetIndex = FMath::RandRange(0, Targets.Num() - 1);
								AFFYBattleCharacter* Target = Targets[TargetIndex];
								Targets.Empty(); 
								Targets.Emplace(Target);
								Result = true;
							}
						
						else
						{
							Result = true;
						}
					}
					
						//5: If Gambit conditions are valid, select and break loop
						if (Result)
						{
							if (Gambits[i].EvaluateOnce) 
							{
								Gambits[i].MaxCooldown = -1; 
							}
							else
							{
								Gambits[i].EvaluateCooldown = Gambits[i].MaxCooldown;
							}
							ConfirmGambit(Gambits[i], Targets);
							break;
						}
					}
					
					//========================================================
				}
			}
		}
		
	}
	else
	{
		return;
	}
	
}

bool UFFYGambitComponent::ValidGambitConditions(AFFYBattleCharacter* Target, FGambit& Gambit)
{
	bool Result = true;
	// HP Floor
	if (Gambit.HPRatioFloor > 0.f)
	{
		if((OwnerCharacter->BattleCharacterStats.HP/OwnerCharacter->BattleCharacterStats.MaxHP) < Gambit.HPRatioFloor)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("%s - GAMBIT SKIP: HP not high enough"), *Gambit.ActionName.ToString()));
			Result = false;
		}
	}
	// HP Limit
	if (Gambit.HPRatioLimit > 0.f)
	{
		if((OwnerCharacter->BattleCharacterStats.HP/OwnerCharacter->BattleCharacterStats.MaxHP) > Gambit.HPRatioLimit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("%s - GAMBIT SKIP: HP too high"), *Gambit.ActionName.ToString()));
			Result = false;
		}
	}
	// Target HP Floor
	if (Gambit.TargetHPRatioFloor > 0.f)
	{
		if((Target->BattleCharacterStats.HP/Target->BattleCharacterStats.MaxHP) < Gambit.TargetHPRatioFloor)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("%s - GAMBIT SKIP: Target's HP not high enough"), *Gambit.ActionName.ToString()));
			Result = false;
		}
	}
	//Target HP Limit
	if (Gambit.TargetHPRatioLimit > 0.f)
	{
		if((Target->BattleCharacterStats.HP/Target->BattleCharacterStats.MaxHP) > Gambit.TargetHPRatioLimit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("%s - GAMBIT SKIP: Target's HP too high"), *Gambit.ActionName.ToString()));
			Result = false;
		}
	}
	//MP Floor
	if (Gambit.MPRatioFloor > 0.f)
	{
		if ((OwnerCharacter->BattleCharacterStats.MP/OwnerCharacter->BattleCharacterStats.MaxMP) < Gambit.MPRatioFloor)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("%s - GAMBIT SKIP: MP too low"), *Gambit.ActionName.ToString()));
			Result = false;
		}
	}
	//MP Limit
	if (Gambit.MPRatioLimit > 0.f)
	{
		if ((OwnerCharacter->BattleCharacterStats.MP/OwnerCharacter->BattleCharacterStats.MaxMP) > Gambit.MPRatioLimit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("%s - GAMBIT SKIP: MP too high"), *Gambit.ActionName.ToString()));
			Result = false;
		}
	}
	//NumEnemies
	if (Gambit.NumEnemies > 0)
	{
		TArray<AActor*> OutEnemies;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Enemy"), OutEnemies);
		if (OutEnemies.Num() < Gambit.NumEnemies)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("%s - GAMBIT SKIP: Too few enemies"), *Gambit.ActionName.ToString()));
			Result = false;
		}
	}
	//NumAllies
	if (Gambit.NumAllies > 0)
	{
		TArray<AActor*> OutParty;
		int Total = 0; 
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Party"), OutParty);
		for (auto p : OutParty)
		{
			IFFYBattleEvents* PartyMember = Cast<IFFYBattleEvents>(p);
			if (PartyMember)
			{
				if (!PartyMember->GetIsDead_Implementation())
				{
					Total++;
				}
			}
		}
		if (Total < Gambit.NumAllies)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("%s - GAMBIT SKIP: Too few party members"), *Gambit.ActionName.ToString()));
			Result = false;
		}
	}
	return Result;
}

void UFFYGambitComponent::ConfirmGambit(FGambit Gambit, TArray<AFFYBattleCharacter*> Targets)
{
	LastSelection = Gambit.ActionName;
	OnGambitSelected.Broadcast(Gambit.ActionName, Gambit);
	OwnerCharacter->ExecuteGambitCommand(Gambit.ActionName, Targets);

	FString Name = OwnerCharacter->GetName();
	FString TName = Targets[0]->GetName();
	FString AName = Gambit.ActionName.ToString();
	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::White, FString::Printf(TEXT("GAMBIT: %s uses %s on %s "), *Name, *AName, *TName));
}

