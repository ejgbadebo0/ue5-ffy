// Source code implementation by Ephraim Gbadebo.


#include "FFYBattleCharacter.h"

#include "FFYAnimationControls.h"
#include "FFYGameInstance.h"
#include "FFY/Components/FFYGambitComponent.h"
#include "FindInBlueprints.h"
#include "Components/WidgetComponent.h"
#include "NiagaraComponent.h"
#include "Components/FFYStatusEffectComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Notify/FFYElementalNiagaraEffectState.h"
#include "Slate/SGameLayerManager.h"
#include "Widgets/Option/FFYBattleCharacterOptionWidget.h"

// Sets default values
AFFYBattleCharacter::AFFYBattleCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//COPIED FROM FFYCHARACTER ==================================
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MaxFlySpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	//====================================================

	InitActorVariables();

}

void AFFYBattleCharacter::InitBattleCharacterStats(FBattleCharacterData& PartyCharacterData, TArray<TSubclassOf<AFFYAction>> AbilityClasses, UFFYActionContainer* Container)
{

	BattleCharacterStats = PartyCharacterData;
	if (Container)
	{
		ActionContainer = Container;
		ActionContainer->OnItemUse.AddUniqueDynamic(this, &AFFYBattleCharacter::ItemUseEvent);
	}
	for (auto a : AbilityClasses)
	{
		if (a)
		{
			AFFYAction* NewAction = GetWorld()->SpawnActor<AFFYAction>(a);
			if (NewAction)
			{
				Actions.Emplace(NewAction);
			}
		}
	}
}

void AFFYBattleCharacter::InitActorVariables()
{
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	if (WeaponMesh)
	{
		WeaponMesh->SetupAttachment(this->GetMesh());
	}
	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
	if (ShieldMesh)
	{
		ShieldMesh->SetupAttachment(this->GetMesh());
	}

	GambitComponent = CreateDefaultSubobject<UFFYGambitComponent>(TEXT("GambitComponent"));

	SelectionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("SelectionWidget"));
	SelectionWidgetComponent->SetupAttachment(this->GetMesh());

	HUDWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HUDWidget"));
	HUDWidgetComponent->SetupAttachment(this->GetMesh());

	DamageEventsWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageEventsWidget"));
	DamageEventsWidget->SetupAttachment(this->GetMesh());
	
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));

	NiagaraSystemComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraSystemComponent"));
	NiagaraSystemComponent->SetupAttachment(this->GetMesh());

	StatusEffectComponent = CreateDefaultSubobject<UFFYStatusEffectComponent>(TEXT("StatusEffectComponent"));
	StatusEffectComponent->SetupAttachment(this->GetMesh());
}


// Called when the game starts or when spawned
void AFFYBattleCharacter::BeginPlay()
{
	Super::BeginPlay();

	//movement mode was initializing to None, needs to be manually set
	GetCharacterMovement()->SetMovementMode(GetCharacterMovement()->DefaultLandMovementMode);
	DefaultTransform = GetActorTransform();
	
	AnimInstance = GetMesh()->GetAnimInstance();
	//PARTY MEMBER:
	InitializeFromPartySlot();

	//Load Equipment Meshes
	
	if (BattleCharacterStats.Equipment.Weapon.ID != NAME_None)
	{
		//FString STR = BattleCharacterStats.Equipment.Weapon.EquipmentData.StaticMesh.ToSoftObjectPath().ToString();
		//GEngine->AddOnScreenDebugMessage(-1, 85.f, FColor::White, FString::Printf(TEXT("REQUESTING LOAD WEAPONS: %s"), *STR));
		WeaponHandle = StreamableManager.RequestAsyncLoad(BattleCharacterStats.Equipment.Weapon.EquipmentData.StaticMesh.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &AFFYBattleCharacter::OnWeaponLoaded),
			FStreamableManager::AsyncLoadHighPriority,
			false);
	}
	if (BattleCharacterStats.Equipment.Shield.ID != NAME_None)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::White, "REQUESTING LOAD SHIELDS");
		ShieldHandle = StreamableManager.RequestAsyncLoad(BattleCharacterStats.Equipment.Shield.EquipmentData.StaticMesh.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &AFFYBattleCharacter::OnShieldLoaded),
			FStreamableManager::AsyncLoadHighPriority,
			false);
	}
	
	//ENEMY:
	InitializeFromEnemyDataTable();
	//Initialize default abilities for enemies/party member data that could not be found. 
	if (Actions.IsEmpty())
	{
		InitDefaultAbilities();
	}


	//WIDGET COMPONENTS:
	SelectionWidget = Cast<UFFYBattleCharacterOptionWidget>(SelectionWidgetComponent->GetWidget());
	if (SelectionWidget)
	{
		//FString Valstr = (ActorHasTag(FName("Enemy"))) ? "true" : "false";
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Black, FString::Printf(TEXT("%s"), *Valstr));
		
		SelectionWidget->Enemy = ActorHasTag(FName("Enemy"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 55.f, FColor::Red, "=============== BC BUTTON INVALID =================");
	}

	//Flying enemies setup
	if (GetCharacterMovement())
	{
		if (GetCharacterMovement()->MovementMode == MOVE_Flying)
		{
			DefaultTransform.SetLocation( FVector(DefaultTransform.GetLocation().X, DefaultTransform.GetLocation().Y, DefaultTransform.GetLocation().Z + FlightHeight));
			SetActorLocation(DefaultTransform.GetLocation());
		}
	}
	
	
}

void AFFYBattleCharacter::InitializeFromPartySlot()
{
	UFFYGameInstance* GameInstance = Cast<UFFYGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance)
	{
		int ACIndex = 0; //corresponding index from GameInstance
		for (auto i : GameInstance->GetParty())
		{
			if (i.PartyCharacterData.CharacterName == BattleCharacterStats.CharacterName)
			{
				//Found party member
				InitBattleCharacterStats(i.PartyCharacterData, i.PartyCharacterAbilities, GameInstance->PartyActions[ACIndex]);
			}
			ACIndex++;
		}
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::White, "BATTLE CHARACTER INITIALIZED");
	}
	//Initialize status effects
	if (StatusEffectComponent)
	{
		for (auto s : BattleCharacterStats.StatusEffects)
		{
			StatusEffectComponent->AddEffect(s);
		}
	}
}

void AFFYBattleCharacter::InitializeFromEnemyDataTable()
{
	if (!EnemyDataHandle.IsNull())
	{
		FBattleCharacterData* Data = EnemyDataHandle.DataTable->FindRow<FBattleCharacterData>(BattleCharacterStats.CharacterName, "", true);
		if (Data)
		{
			BattleCharacterStats = *Data;
		}

		if (GambitComponent)
		{
			GambitComponent->SetGambitsActive(true);
		}
	}
}

void AFFYBattleCharacter::ItemUseEvent(FName ID, AFFYBattleCharacter* User, AFFYBattleCharacter* Target)
{
	IFFYBattleEvents* GameInstance = Cast<IFFYBattleEvents>(GetWorld()->GetGameInstance());
	if (GameInstance)
	{
		GameInstance->UseInventoryItem_Implementation(ID, User, Target);
	}
}

void AFFYBattleCharacter::InitDefaultAbilities()
{
	ActionContainer = NewObject<UFFYActionContainer>(this, UFFYActionContainer::StaticClass());
	for (auto a : DefaultAbilityClasses)
	{
		if (a)
		{
			AFFYAction* NewAction = GetWorld()->SpawnActor<AFFYAction>(a);
			if (NewAction)
			{
				ActionContainer->AddAction(NewAction);
			}
		}
	}
}

// Called every frame
void AFFYBattleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFFYBattleCharacter::OnFreeActionUse()
{
	bIsFocused = false;
	OnFocusedStateChanged.Broadcast(this, bIsFocused);
}

void AFFYBattleCharacter::OnWeaponLoaded()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::White, "WEAPON LOADED");
	
	UObject* Object = WeaponHandle->GetLoadedAsset();
	UStaticMesh* LoadedMesh = Cast<UStaticMesh>(Object);
	if (LoadedMesh)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::White, "WEAPONMESH LOADED");
		WeaponMesh->SetStaticMesh(LoadedMesh);
		WeaponMesh->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponSocketName);
	}
	else
	{
		
	}
}

void AFFYBattleCharacter::OnShieldLoaded()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::White, "SHIELD LOADED");
	UObject* Object = ShieldHandle->GetLoadedAsset();
	UStaticMesh* LoadedMesh = Cast<UStaticMesh>(Object);
	if (LoadedMesh)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::White, "SHIELNMESH LOADED");
		ShieldMesh->SetStaticMesh(LoadedMesh);
		ShieldMesh->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ShieldSocketName);
	}
}

void AFFYBattleCharacter::Consume(float ATBCost, float HPCost, float MPCost)
{
	if (ATBCost != 0)
	{
		ATB -= ATBCost;
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::Printf(TEXT("%s: %f"), *BattleCharacterStats.CharacterName.ToString(), ATB));
		OnATBValueChanged.Broadcast(ATB);
	}
	if (HPCost != 0)
	{
		
	}
	if (MPCost != 0)
	{
		BattleCharacterStats.MP -= MPCost;
		OnCharacterStatsChanged.Broadcast(BattleCharacterStats);
	}
}

void AFFYBattleCharacter::UpdateATB(float DeltaTimeX)
{
	if (ATB == 100.f || ActionState == EActionState::ACTING || ActiveState == EActiveState::WAIT)
	{
		return;
	}

	// not max so increase ATB according to current status
	float ATBFactor = 5.f + (BattleCharacterStats.Dexterity/100.f);

	if (BattleCharacterStats.StatusEffects.IsEmpty())
	{
		ATB = FMath::Clamp(ATB + (ATBFactor * DeltaTimeX), 0.f, 100.f);
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, FString::Printf(TEXT("%s: %f"), *BattleCharacterStats.CharacterName.ToString(), ATB));
		OnATBValueChanged.Broadcast(ATB);
	}
	else
	{
		for (auto s : BattleCharacterStats.StatusEffects)
		{
			float* TimeModifier = TimeAlteringEffects.Find(s);
			if (TimeModifier)
			{
				ATBFactor *= *TimeModifier;
			}
		}
		
		ATB = FMath::Clamp(ATB + (ATBFactor * DeltaTimeX), 0.f, 100.f);
		if (ATBFactor > 0.f)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, FString::Printf(TEXT("%s: %f"), *BattleCharacterStats.CharacterName.ToString(), ATB));
			OnATBValueChanged.Broadcast(ATB);
		}
	}
	
}

void AFFYBattleCharacter::UpdateDamageAttributes(float DamageAmount, EStat DamageStat, EDamageModifier DamageModifier,
                                                 EElement DamageElement, TArray<EStatusEffect> Inflicts, TArray<EStatusEffect> Removes, float InnateModifier,
                                                 EAttackType AttackType)
{
	DamageAttributes.DamageAmount = DamageAmount;
	DamageAttributes.DamageStat = DamageStat;
	DamageAttributes.DamageModifier = DamageModifier;
	DamageAttributes.DamageElement = DamageElement;
	DamageAttributes.Inflicts = Inflicts;
	DamageAttributes.Removes = Removes;
	DamageAttributes.InnateModifier = InnateModifier;
	DamageAttributes.AttackType = AttackType;
}

void AFFYBattleCharacter::UpdateMotionWarpTransform(FName WarpName, FVector WarpLocation, FRotator WarpRotation) const
{
	if (MotionWarpingComponent)
	{
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(WarpName, WarpLocation, WarpRotation);
	}
}

FTransform AFFYBattleCharacter::GetWarpTargetTransform(FName WarpName) const
{
	FTransform WarpTransform = FTransform::Identity;
	if (MotionWarpingComponent)
	{
		const FMotionWarpingTarget* WarpingTarget = MotionWarpingComponent->FindWarpTarget(WarpName);
		if (WarpingTarget)
		{
			WarpTransform = WarpingTarget->GetTargetTrasform();
		}
	}
	return WarpTransform;
}

void AFFYBattleCharacter::OnParry()
{
	//refund 50 ATB to allow another Defend
	ATB = FMath::Min(ATB + 50.f, 100.f);
	UpdateATB(0.001f);

	//update chain
	IFFYBattleEvents* Manager = Cast<IFFYBattleEvents>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Manager)
	{
		int Chain = Manager->GetChain_Implementation() + 1;
		Manager->UpdateChain_Implementation(Chain, 1.f);
	}
}

void AFFYBattleCharacter::PruneStatusEffects()
{
	if (BattleCharacterStats.StatusEffects.IsEmpty())
	{
		return;
	}
	for (auto s : BattleCharacterStats.StatusEffects)
	{
		if (s != EStatusEffect::KO)
		{
			if (StatusEffectComponent)
			{
				StatusEffectComponent->RemoveEffect(s);
			}
			BattleCharacterStats.StatusEffects.RemoveSingle(s);
		}
	}
}

// Executed once a valid hit is evaluated
FDamageEventResult AFFYBattleCharacter::InflictDamage(const FDamageAttributes& SourceDamage, int SourceLevel, float CriticalModifier)
{
	//GEngine->AddOnScreenDebugMessage(-1, 35.f, FColor::Orange, "DAMAGE: =====================" );
	//GEngine->AddOnScreenDebugMessage(-1, 35.f, FColor::Red, FString::Printf(TEXT("Base Damage Amount: %f"), SourceDamage.DamageAmount ));
	

	FText ResultText = FText::GetEmpty();
	float DefenseStat = 0.f;
	
	switch (SourceDamage.DamageModifier)
	{
	default:
		break;
	case EDamageModifier::NONE: //damage from source other than an attack
		CriticalModifier = 0.f;
		break;
	case EDamageModifier::MAGIC:
		DefenseStat = BattleCharacterStats.Spirit;
		break;
	case EDamageModifier::PHYSICAL:
		DefenseStat = BattleCharacterStats.Constitution;
		if (SourceDamage.AttackType == EAttackType::MELEE && bCanClash) //if phys/melee attack hits while in clashing state from animation
		{
			bHasClashed = true;
			OnClashEvent(); 
			return FDamageEventResult(true,
			false,
			false,
			PerfectDefense,
			-1,
			ResultText,
			SourceDamage);
		}
		break;
	}

	//modifier for base defense stats
	float StatModifier = DefenseStat/2;
	
	//modifiers for critical hit chance
	float CrtNum = FMath::RandRange(0.f, 1.f);
	bool bIsCrit = (CriticalModifier > CrtNum);
	float CritDmgModifier = ((bIsCrit)) ? 1.5f : 1.f; //50% increase to final damage if critical
	
	GEngine->AddOnScreenDebugMessage(-1, 35.f, (bIsCrit) ? FColor::Yellow : FColor::White, FString::Printf(TEXT("CRIT ===:%f > %f"), CriticalModifier, CrtNum ));
	
	//multiplied to decrease the final damage amount based on defense variables
	float DefendModifier = (this->ActionState == EActionState::DEFENDING && SourceDamage.DamageModifier != EDamageModifier::NONE)
				? 0.75f * //if defending decrease damage taken by 25% at base
					((PerfectDefense) ? ((ShieldMesh->IsVisible()) ? 0.0f : 0.1f) : (1 - DefenseFactor)) //if a perfect timed defense, decrease to ~7% and 0 if equipped with a shield, else use the DefenseFactor derived from ATB consumed when beginning defend.
				: 1.f; //take full damage if not defending
	
	//GEngine->AddOnScreenDebugMessage(-1, 35.f, FColor::Red, FString::Printf(TEXT("Defend: %f"), DefendModifier ));
	
	//modifier for damage amount based on difference in levels
	float LevelDamageModifier =  (SourceDamage.DamageAmount != 0.f) ? SourceDamage.DamageAmount + ((SourceLevel - BattleCharacterStats.LV) * 2) : 0.f; 
	//GEngine->AddOnScreenDebugMessage(-1, 35.f, FColor::Red, FString::Printf(TEXT("LevelDmgModifier: %f"), LevelDamageModifier ));
	
	float FinalDamage = FMath::Floor(FMath::Clamp(((LevelDamageModifier - StatModifier) * DefendModifier) * CritDmgModifier, 0.f, DamageCap));
	//GEngine->AddOnScreenDebugMessage(-1, 35.f, FColor::Red, FString::Printf(TEXT("DAMAGE CALC: (%f - %f) * %f = %f"), LevelDamageModifier, StatModifier, DefendModifier, FinalDamage ));
	//GEngine->AddOnScreenDebugMessage(-1, 35.f, FColor::Red, FString::Printf(TEXT("FinalDamage: %f"), FinalDamage ));
	//GEngine->AddOnScreenDebugMessage(-1, 35.f, FColor::Orange, "=========================" );

    float PreDamageHPRatio = (BattleCharacterStats.HP/BattleCharacterStats.MaxHP);
	//DEAL FINAL DAMAGE ==========================================
	BattleCharacterStats.HP = FMath::Max(BattleCharacterStats.HP - FinalDamage, (PerfectDefense) ? 1.f : 0.0f);
	
	if (BattleCharacterStats.HP <= 0)
	{
		PruneStatusEffects();
		BattleCharacterStats.StatusEffects.Emplace(EStatusEffect::KO);
		
		FTextFormat FMT = FTextFormat::FromString("{Previous}<Fatal>KO</>\n");
		FFormatNamedArguments Args;
						
		Args.Add("Previous", ResultText);
		
		ResultText = FText::Format(FMT, Args);
		IFFYAnimationControls* Instance = Cast<IFFYAnimationControls>(AnimInstance);
		if (Instance)
		{
			Instance->PlayActionMontage_Implementation(FName("KO"), false);
		}
		
		if (StatusEffectComponent)
		{
			StatusEffectComponent->AddEffect(EStatusEffect::KO);
			StatusEffectComponent->OnKOEffectFinished.AddUniqueDynamic(this, &AFFYBattleCharacter::K2_DestroyActor);
		}
		OnCharacterDefeated.Broadcast(this);
		if (ActorHasTag(FName("Enemy")))
		{
			EnemyKOEvent();
		}
	}
	else 
	{

		//defense animation montages
		IFFYAnimationControls* Instance = Cast<IFFYAnimationControls>(AnimInstance);
		
		switch (this->ActionState)
		{
		default:
			break;
		case EActionState::DEFENDING:
			if (Instance && !(SourceDamage.DamageElement == EElement::STATUS && SourceDamage.DamageModifier == EDamageModifier::MAGIC) && (SourceDamage.DamageModifier != EDamageModifier::NONE)) //don't allow parrying on pure status inflicting moves
			{
				Instance->PlayActionMontage_Implementation((PerfectDefense) ? FName("Parry") : FName("Block"), false);
				if (PerfectDefense)
				{
					OnParry();
				}
			}
			break;
		case EActionState::IDLE:
			if (Instance
				&& FVector::Distance(GetActorLocation(), DefaultTransform.GetLocation()) <= 20.f //prevent repositioning to be overriden by anim
				&& !(AnimInstance->IsAnyMontagePlaying()) //prevent cancelling ongoing action
				&& FinalDamage > 0.f) //don't play if no damage was received
			{
				Instance->PlayActionMontage_Implementation(FName("Damage"), false);
			}
			break;
		}
		
		//alert that character is low HP for specific abilities
		if ((BattleCharacterStats.HP/BattleCharacterStats.MaxHP) <= 0.25f && PreDamageHPRatio > 0.25f)
		{
			OnCharacterWeakened.Broadcast(this);
		}
	}
	//=============================================================


	
	//If damage source also inflicts a status
	
	if (!SourceDamage.Inflicts.IsEmpty())
	{
		//evaluate each status
		for (auto s : SourceDamage.Inflicts)
		{
			//skip if natural resistance to this status or KO'd
			if (!BattleCharacterStats.Immunities.Contains(s) && !BattleCharacterStats.StatusEffects.Contains(EStatusEffect::KO))
			{
				//determine probability of infliction with stats
				if (ResistanceTypes.Contains(s))
				{
					float ResistStat = 0.f;
					EDamageModifier ResistType = *ResistanceTypes.Find(s);
				
					switch (ResistType)
					{
					case EDamageModifier::PHYSICAL:
						ResistStat = BattleCharacterStats.Constitution;
						break;
					case EDamageModifier::MAGIC:
						ResistStat = BattleCharacterStats.Spirit;
						break;
					}

					float ResistModifier = ResistStat/256;
					float LuckModifier = BattleCharacterStats.Luck/100.f;

					float ResistChance = ResistModifier + LuckModifier;
					float InflictChance = (PerfectDefense) ? SourceDamage.InnateModifier : FMath::RandRange(0.f, 1.f) +  SourceDamage.InnateModifier;

					//add inflicted status ailment
					if (InflictChance > ResistChance && !(BattleCharacterStats.StatusEffects.Contains(s)))
					{
						BattleCharacterStats.StatusEffects.Add(s);

						StatusInflictedEvent(s);
						if (StatusEffectComponent)
						{
							StatusEffectComponent->AddEffect(s);
						}
						//check if it's a condition that prevents further action
						if (LossConditionEffects.Contains(s))
						{
							OnCharacterDefeated.Broadcast(this);
						}
					}
					
					//add result to final text result
					FText StatusText;
					UEnum::GetDisplayValueAsText(s, StatusText);
					//format for richtexet
					FTextFormat FMT = FTextFormat::FromString("{Previous}{S}{Status}{E}\n");
					FFormatNamedArguments Args;
						
					Args.Add("Previous", ResultText);
					Args.Add("S", (InflictChance > ResistChance) ? ( (s == EStatusEffect::DEATH) ? FText::FromString("<Fatal>"): FText::FromString("<Debuff>")) : FText::GetEmpty());
					Args.Add("E", (InflictChance > ResistChance) ? FText::FromString("</>") : FText::GetEmpty());
					Args.Add("Status",(InflictChance > ResistChance) ? StatusText : FText::FromString("Resist"));
						
					ResultText = FText::Format(FMT, Args);
				}
			}
			else
			{
				//update text result to notify that status was naturally resisted
				FTextFormat FMT = FTextFormat::FromString("{Previous}Resist\n");
				FFormatNamedArguments Args;
				Args.Add("Previous", ResultText);
				ResultText = FText::Format(FMT, Args);
			}
			
		}
	}

	OnCharacterStatsChanged.Broadcast(BattleCharacterStats);
	
	return FDamageEventResult(true,
		bIsCrit,
		false,
		PerfectDefense,
		FinalDamage,
		ResultText,
		SourceDamage);
}

//Custom Damage Functions
bool AFFYBattleCharacter::ReceiveDamage_Implementation(
	AFFYBattleCharacter* Source,
	AFFYAction* SourceAction)
{
	//determine if valid
	if (!Source)
	{
		return false;
	}
	//determine if has clashed with a character
	if (Source->bHasClashed)
	{
		return false;
	}
	//determine if character can currently take damage
	if (!BattleCharacterStats.StatusEffects.IsEmpty())
	{
		for (auto s : BattleCharacterStats.StatusEffects)
		{
			if (UndamageableEffects.Contains(s))
			{
				return false;
			}
		}
	}

	DamageSource = Source;
	FDamageAttributes SourceDamageAttributes = Source->GetDamageAttributes_Implementation();

	/*
	float DamageAmount = 0;
	EStat DamageStat = EStat::HP;
	EDamageModifier DamageModifier = EDamageModifier::NONE;
	EElement DamageElement = EElement::NONE;
	TArray<EStatusEffect> Inflicts;
	TArray<EStatusEffect> Removes;
	EAttackType AttackType = EAttackType::NONE;
	 */

	FDamageEventResult DamageEventResult;
	
	switch (SourceDamageAttributes.DamageModifier)
	{
		default:
			break;
		case EDamageModifier::MAGIC: //magic attacks will always hit for now

			DamageEventResult = InflictDamage(SourceDamageAttributes, Source->BattleCharacterStats.LV, 0);
			DamageTakenEvent(DamageEventResult);
			return true;
		
		case EDamageModifier::PHYSICAL:
			if (SourceDamageAttributes.DamageAmount > 0)
			{
				//set baseline hit probability
				float HitBase = (Source->BattleCharacterStats.StatusEffects.Contains(EStatusEffect::BLIND)) ? 0.15f : 1.f;

				//evaluate modifiers based on precision&dex. Clamped to (0.01 - 2)
				float AccuracyModifier = FMath::Clamp((FMath::Max(1.f, Source->BattleCharacterStats.Precision)/FMath::Max(1.f,  BattleCharacterStats.Dexterity)), 0.001f, 2.f);

				//luck
				float LuckModifier = FMath::Clamp(BattleCharacterStats.Luck - Source->BattleCharacterStats.Luck, 0.f, 1.f);

				float CritModifier = FMath::Min((Source->BattleCharacterStats.Precision/255), 0.2) + (Source->BattleCharacterStats.Luck/100.f);

				//final probability
				float Hit = FMath::Clamp(((HitBase * AccuracyModifier) - LuckModifier), 0.f, 1.f);
				float Miss = FMath::RandRange(0.f, 1.f);

				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, "==========================");
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("HIT RESULT -> Hit: %f / Miss: %f / Crit: %f"), Hit, Miss, CritModifier));
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("HIT MODIFIERS -> (%f * %f) - %f"), HitBase, AccuracyModifier, LuckModifier));
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, "==========================");
				//Successful hit from physical attack
				if (Hit > Miss)
				{
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(FString("DAMAGE TAKEN -> Hit: %f / Evade: %f"), Hit, Evade));	

					DamageEventResult = InflictDamage(SourceDamageAttributes, Source->BattleCharacterStats.LV, CritModifier);
					DamageEventResult.SourceLocation = Source->GetActorLocation();
					DamageTakenEvent(DamageEventResult);
					return true;
				}
				break;
			}
		
	}

	//reached due to miss
	FText ResultText = (PerfectDefense) ? FText::FromString("") : FText::FromString("Miss");
	//GEngine->AddOnScreenDebugMessage(-1, 35.f, FColor::Orange, "MISSED" );
	if (PerfectDefense && !Source->BattleCharacterStats.StatusEffects.Contains(EStatusEffect::BLIND)) //perfect evade condition, don't reward on blind enemies
	{
		OnPerfectEvade();
		bIsFocused = true;
		OnFocusedStateChanged.Broadcast(this, bIsFocused);
	}

	DamageEventResult.bWasHit = false;
	DamageEventResult.bIsCrit = false;
	DamageEventResult.bIsHealing = false;
	DamageEventResult.bIsPerfectDefense = PerfectDefense;
	DamageEventResult.DamageReceived = 0;
	DamageEventResult.ResultDescription = ResultText;
	DamageEventResult.InDamageAttributes = SourceDamageAttributes;
	
	DamageTakenEvent(DamageEventResult);
	
	return false;
}

bool AFFYBattleCharacter::ReceiveHealing_Implementation(AFFYBattleCharacter* Source, AFFYAction* SourceAction)
{
	//determine if valid
	if (!Source)
	{
		return false;
	}

	FDamageEventResult DamageEventResult;
	FDamageAttributes SourceDamageAttributes = Source->GetDamageAttributes_Implementation();
	EElement HealingEffectType = SourceDamageAttributes.DamageElement;
	FText ResultText = FText::GetEmpty();
	
	switch (HealingEffectType)
	{
	default:
		return false;
	case EElement::HEALING: //purely healing
		//determine if not in a heal-able state
		if (BattleCharacterStats.StatusEffects.Contains(EStatusEffect::KO) || BattleCharacterStats.StatusEffects.Contains(EStatusEffect::PETRIFY))
		{
			return false;
		}

		//if Zombie type enemy or affected by Zombie status deal damage to target instead. 
		if (BattleCharacterStats.StatusEffects.Contains(EStatusEffect::ZOMBIE) || BattleCharacterStats.Weaknesses.Contains(EElement::HEALING))
		{
			return ReceiveDamage_Implementation(Source, SourceAction);
		} 
		
		BattleCharacterStats.HP = FMath::Floor(FMath::Min(BattleCharacterStats.HP + SourceDamageAttributes.DamageAmount, BattleCharacterStats.MaxHP));

		OnCharacterStatsChanged.Broadcast(BattleCharacterStats);
		
		DamageEventResult.bWasHit = true;
		DamageEventResult.bIsHealing = true;
		DamageEventResult.DamageReceived = SourceDamageAttributes.DamageAmount;
		
		DamageTakenEvent(DamageEventResult);
		return true;
		
	case EElement::STATUS: //status removal and possible buff/healing
		
		if (SourceDamageAttributes.Removes.IsEmpty())
		{
			return false;
		}

		bool RemoveResult = false;
		
		for (auto s : SourceDamageAttributes.Removes)
		{
			if (BattleCharacterStats.StatusEffects.Contains(s))
			{
				BattleCharacterStats.StatusEffects.Remove(s);
				StatusRemovedEvent(s);
				
				if (StatusEffectComponent)
				{
					StatusEffectComponent->RemoveEffect(s);
				}
				RemoveResult = true;
			}
		}

		//determine if not in a heal-able/buffable state or didn't remove the ailment and return if not
		if ( !RemoveResult || BattleCharacterStats.StatusEffects.Contains(EStatusEffect::KO) || BattleCharacterStats.StatusEffects.Contains(EStatusEffect::PETRIFY))
		{
			DamageEventResult.bWasHit = false;
			DamageEventResult.bIsHealing = true;
			DamageEventResult.bIsPerfectDefense = false;
			DamageEventResult.DamageReceived = 0;
			DamageEventResult.ResultDescription = FText::FromString("Miss");
			DamageEventResult.InDamageAttributes = SourceDamageAttributes;
			
			DamageTakenEvent(DamageEventResult);
			return false;
		}

		//buffs

		if (!SourceDamageAttributes.Inflicts.IsEmpty())
		{
			for (auto s : SourceDamageAttributes.Inflicts)
			{
				if (!BattleCharacterStats.StatusEffects.Contains(s))
				{
					BattleCharacterStats.StatusEffects.Emplace(s);

					FText StatusText;
					UEnum::GetDisplayValueAsText(s, StatusText);
					//format for richtexet
					FTextFormat FMT = FTextFormat::FromString("{Previous}<Buff>{Status}</>\n");
					FFormatNamedArguments Args;
						
					Args.Add("Previous", ResultText);
					Args.Add("Status",StatusText);
						
					ResultText = FText::Format(FMT, Args);
				}
			}
		}
		
		BattleCharacterStats.HP = FMath::Floor(FMath::Min(BattleCharacterStats.HP + SourceDamageAttributes.DamageAmount, BattleCharacterStats.MaxHP));

		OnCharacterStatsChanged.Broadcast(BattleCharacterStats);
		
		DamageEventResult.bWasHit = true;
		DamageEventResult.bIsHealing = true;
		DamageEventResult.bIsPerfectDefense = false;
		DamageEventResult.DamageReceived = SourceDamageAttributes.DamageAmount;
		DamageEventResult.ResultDescription = ResultText;
		DamageEventResult.InDamageAttributes = SourceDamageAttributes;
			
		DamageTakenEvent(DamageEventResult);
		return true;
		
	}
}

void AFFYBattleCharacter::SetVictoryState()
{
	bool SetCondition = true;
	if (bIsFocused) //make sure other actors are unpaused
	{
		OnFreeActionUse();
	}
	for (auto e : BattleCharacterStats.StatusEffects) //make sure in a state that actor can play victory animation
	{
		if (EXPGainDisablingEffects.Contains(e))
		{
			SetCondition = false;
		}
	}

	bVictoryState = SetCondition;
}

void AFFYBattleCharacter::ActionUsed_Implementation(FName ActionName, bool bIsEnemy)
{
	IFFYBattleEvents* Pawn = Cast<IFFYBattleEvents>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
			
	if (Pawn != nullptr)
	{
		Pawn->ActionUsed_Implementation(ActionName, bIsEnemy);
	}
}

//=====================
