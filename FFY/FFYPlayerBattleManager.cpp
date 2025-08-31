// Source code implementation by Ephraim Gbadebo.


#include "FFYPlayerBattleManager.h"

#include "FFYBattleGameMode.h"
#include "FFYBattleSpawnContainer.h"
#include "FFYBattleSpawnTransform.h"
#include "FFYGameInstance.h"
#include "Components/WrapBox.h"
#include "GameFramework/SpringArmComponent.h"
#include "Widgets/FFYBattleWidget.h"
#include "Widgets/FFYActionWidget.h"
#include "Widgets/FFYBattleResultsWidget.h"

// Sets default values
AFFYPlayerBattleManager::AFFYPlayerBattleManager()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//initialize

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	MainCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	MainCamera->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void AFFYPlayerBattleManager::BeginPlay()
{
	Super::BeginPlay();

	DefaultTransform = GetActorTransform();
	
	//Create widget and initialize variables
	UFFYGameInstance* GameInstance = Cast<UFFYGameInstance>(GetGameInstance());
	AFFYBattleSpawnContainer* SpawnContainer = Cast<AFFYBattleSpawnContainer>(UGameplayStatics::GetActorOfClass(GetWorld(), AFFYBattleSpawnContainer::StaticClass()));
	BattleWidget = CreateWidget<UFFYBattleWidget>(GetWorld(), MasterWidgetClass);
	ActionWidget = CreateWidget<UFFYActionWidget>(GetWorld(), ActionWidgetClass);

	if (GameInstance && SpawnContainer)
	{
		//hard cap at 4 active members or number of slots in battle arena
		for (int i = 0; i < 4 && i < GameInstance->GetParty().Num() && i < SpawnContainer->PartySpawns.Num(); i++)
		{
			AFFYBattleCharacter* PartySpawn = GetWorld()->SpawnActor<AFFYBattleCharacter>(GameInstance->GetParty()[i].PartyCharacterData.BattleCharacterClass, SpawnContainer->PartySpawns[i]->GetActorTransform());
			if (PartySpawn && BattleWidget)
			{
				Party.Emplace(PartySpawn);
				//Bind party member delegates
				PartySpawn->OnCharacterDefeated.AddUniqueDynamic(this, &AFFYPlayerBattleManager::CheckLossCondition);
				PartySpawn->OnWaitActionUsed.AddUniqueDynamic(this, &AFFYPlayerBattleManager::SetBattleActiveState);
				PartySpawn->OnFocusedStateChanged.AddUniqueDynamic(this, &AFFYPlayerBattleManager::OnFocusStateChanged);
				PartySpawn->OnCameraActionSelected.AddUniqueDynamic(this, &AFFYPlayerBattleManager::OnCameraActionSelected);

				//===============
				BattleWidget->AddHUDSlot(PartySpawn);
			}
		}
		BattleWidget->InitializeContextMenu(Party);
	}
	AFFYBattleGameMode* GameMode = Cast<AFFYBattleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->OnEnemiesLoaded.AddUniqueDynamic(this, &AFFYPlayerBattleManager::UpdateEnemies);
	GameMode->OnBattleStarted.AddUniqueDynamic(this, &AFFYPlayerBattleManager::OnStartBattle);

	if (ActionWidget)
	{
		ActionWidget->AddToViewport();
	}
	
	if (BattleWidget)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Black, "BATTLE WIDGET CONSTRUCT");
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController)
		{
			PlayerController->SetShowMouseCursor(true);
		}

		if (Party[0])
		{
			BattleWidget->LoadBattleContext_Implementation(Party[0]);
			BattleWidget->AddToViewport();
		}
	}
}

float AFFYPlayerBattleManager::FindHighestDexterity() const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFFYBattleCharacter::StaticClass(), FoundActors);

	float HighestDexterity = 0.0f;
	
	for (auto a : FoundActors)
	{
		AFFYBattleCharacter* Character = Cast<AFFYBattleCharacter>(a);
		if (Character && Character->BattleCharacterStats.Dexterity > HighestDexterity)
		{
			HighestDexterity = Character->BattleCharacterStats.Dexterity;
		}
	}
	
	return HighestDexterity;
}

// Called every frame
void AFFYPlayerBattleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bHasBattleStarted)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, "Battle Ongoing");
		for (auto p : Party)
		{
			if (p)
			{
				p->UpdateATB(DeltaTime);
			}
		}
		for (auto e : Enemies)
		{
			if (e)
			{
				e->UpdateATB(DeltaTime);
			}
		}
	}

}
	

//INPUT ACTIONS:

void AFFYPlayerBattleManager::Move(const FInputActionValue& Value)
{
	
}

void AFFYPlayerBattleManager::Confirm(const FInputActionValue& Value)
{
	if (BattleResultsWidget)
	{
		if (BattleResultsWidget->IsVisible())
		{
			BattleResultsWidget->Advance();
		}
	}
}

void AFFYPlayerBattleManager::Cancel(const FInputActionValue& Value)
{
	if (BattleWidget)
	{
		switch (BattleWidget->GetCurrentMenuMode())
		{
		default:
			break;
		case EMenuMode::NONE:
			BattleWidget->PreviousMenu();
			break;
		case EMenuMode::SELECTING:
			IFFYWidgetEvents* BattleWidgetInterface = Cast<IFFYWidgetEvents>(BattleWidget);
			if (BattleWidgetInterface)
			{
				BattleWidgetInterface->EndSelection_Implementation();
			}
			break;
		}
		
	}
}

void AFFYPlayerBattleManager::Cycle(const FInputActionValue& Value)
{
	//change to get context character
	if (BattleWidget)
	{
		IFFYWidgetEvents* BattleWidgetInterface = Cast<IFFYWidgetEvents>(BattleWidget);
		if (BattleWidgetInterface)
		{
			PartyIndex += FMath::Floor(Value.GetMagnitude());
			if (PartyIndex >= Party.Num())
			{
				PartyIndex = 0;
			}
			if (PartyIndex < 0)
			{
				PartyIndex = Party.Num() - 1;
			}
			BattleWidgetInterface->LoadBattleContext_Implementation(Party[PartyIndex]);
		}
	}
}

void AFFYPlayerBattleManager::Pause(const FInputActionValue& Value)
{
	
}

void AFFYPlayerBattleManager::SelectMode(const FInputActionValue& Value)
{
	if (BattleWidget)
	{
		IFFYWidgetEvents* Widget = Cast<IFFYWidgetEvents>(BattleWidget->GetWidgetSwitcher()->GetActiveWidget());
		if (Widget)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "SELECTMODE INPUT");
			Widget->SelectAllInputEvent_Implementation();
		}
	}
}

void AFFYPlayerBattleManager::SwitchTargets(const FInputActionValue& Value)
{
	if (BattleWidget)
	{
		IFFYWidgetEvents* Widget = Cast<IFFYWidgetEvents>(BattleWidget->GetWidgetSwitcher()->GetActiveWidget());
		if (Widget)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "SWITCHTARGETS INPUT");
			Widget->SwitchTargetsInputEvent_Implementation();
		}
	}
}

void AFFYPlayerBattleManager::Defend(const FInputActionValue& Value)
{
	if (BattleWidget)
	{
		IFFYWidgetEvents* Widget = Cast<IFFYWidgetEvents>(BattleWidget);
		if (Widget)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "SELECTMODE INPUT");
			Widget->DefendInputEvent_Implementation();
		}
	}
}

void AFFYPlayerBattleManager::ContextCommand(const FInputActionValue& Value)
{

	if (BattleWidget)
	{
		IFFYWidgetEvents* Widget = Cast<IFFYWidgetEvents>(BattleWidget);
		if (Widget)
		{
			Widget->ContextCommandInputEvent_Implementation(FMath::Floor(Value.GetMagnitude()));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CONTEXTCOMMAND INPUT: %f"), Value.GetMagnitude()));
		}
	}
}


//------------
// Called to bind functionality to input
void AFFYPlayerBattleManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFFYPlayerBattleManager::Move);
		

		// Custom Actions
		EnhancedInputComponent->BindAction(ConfirmAction, ETriggerEvent::Started, this, &AFFYPlayerBattleManager::Confirm);
		EnhancedInputComponent->BindAction(CancelAction, ETriggerEvent::Started, this, &AFFYPlayerBattleManager::Cancel);
		EnhancedInputComponent->BindAction(CycleAction, ETriggerEvent::Started, this, &AFFYPlayerBattleManager::Cycle);
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &AFFYPlayerBattleManager::Pause);
		EnhancedInputComponent->BindAction(SelectModeAction, ETriggerEvent::Started, this, &AFFYPlayerBattleManager::SelectMode);
		EnhancedInputComponent->BindAction(SwitchTargetsAction, ETriggerEvent::Started, this, &AFFYPlayerBattleManager::SwitchTargets);
		EnhancedInputComponent->BindAction(DefendAction, ETriggerEvent::Started, this, &AFFYPlayerBattleManager::Defend);
		EnhancedInputComponent->BindAction(ContextCommandAction, ETriggerEvent::Started, this, &AFFYPlayerBattleManager::ContextCommand);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

}

void AFFYPlayerBattleManager::CheckLossCondition(AFFYBattleCharacter* Character)
{

	bool Result = true;
	
	if (Character->bIsFocused) //if in focused state, cancel
	{
		Character->OnFreeActionUse();
	}
	
	for (auto p : Party) //is there still a party member able to act
	{
		bool Condition = false;
		if (p)
		{
			for (auto f : p->LossConditionEffects)
			{
				if (p->BattleCharacterStats.StatusEffects.Contains(f))
				{
					Condition = true;
				}
			}
			if (!Condition)
			{
				Result = false;
				return;
			}
		}
	}
	if (Result)
	{
		//loss
		GEngine->AddOnScreenDebugMessage(-1, 55.f, FColor::Black, "GAME OVER CONDITION: All party members defeated");
		Defeat();
	}

	
}

void AFFYPlayerBattleManager::CheckWinCondition(AFFYBattleCharacter* Character)
{
	float EXPToAdd = Character->BattleCharacterStats.EXP;
	PartyEXPGained += EXPToAdd;

	if (Enemies.Contains(Character))
	{
		if (!(Enemies.Num() > 1)) //is this the last enemy
		{
			//win
			GEngine->AddOnScreenDebugMessage(-1, 55.f, FColor::White, "VICTORY CONDITION REACHED: No enemies left");
			if (BattleWidget)
			{
				BattleWidget->BattleEnd();
			}
			GetWorld()->GetTimerManager().SetTimer(VictoryTimer, this, &AFFYPlayerBattleManager::Victory, 3.f);
		}
		else
		{
			for (auto e : Enemies) //are all other enemies unable to act
			{
				bool Condition = false;
				if (e)
				{
					for (auto f : e->LossConditionEffects)
					{
						if (e->BattleCharacterStats.StatusEffects.Contains(f))
						{
							Condition = true;
						}
					}
					if (!Condition)
					{
						return;
					}
				}
			}

			//win
			GEngine->AddOnScreenDebugMessage(-1, 55.f, FColor::White, "VICTORY CONDITION REACHED: Enemies can't act");
			if (BattleWidget)
			{
				BattleWidget->BattleEnd();
			}
			GetWorld()->GetTimerManager().SetTimer(VictoryTimer, this, &AFFYPlayerBattleManager::Victory, 3.f);
		}
	}
}

void AFFYPlayerBattleManager::Victory()
{


	IFFYBattleEvents* GameInstance = Cast<IFFYBattleEvents>(GetGameInstance());
	BattleResultsWidget = CreateWidget<UFFYBattleResultsWidget>(GetWorld(), BattleResultsWidgetClass);
	if (BattleResultsWidget && GameInstance)
	{
		
		BattleResultsWidget->AddToViewport();
		
		for (auto p : Party)
		{
			//apply exp reward to character and replicate data to results screen
			FBattleEXPData EXPResult = CalculateEXPGained(p);
			LevelUp(p, EXPResult);
			BattleResultsWidget->AddEXPSlot(EXPResult);

			//replicate changes to persistent character data
			GameInstance->UpdatePartyMemberStats_Implementation(p->BattleCharacterStats);

			p->SetVictoryState();
		
		}

		BattleResultsWidget->OnResultScreenExit.AddUniqueDynamic(this, &AFFYPlayerBattleManager::BattleEnded);
	}
	
	


}

void AFFYPlayerBattleManager::Defeat()
{
}

void AFFYPlayerBattleManager::BattleEnded()
{
	IFFYBattleEvents* GameInstance = Cast<IFFYBattleEvents>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->EndBattle_Implementation();
	}
}

FBattleEXPData AFFYPlayerBattleManager::CalculateEXPGained(AFFYBattleCharacter* Character)
{
	FBattleEXPData EXPDataResult = FBattleEXPData();
	
	if (Character)
	{
		//set starter values
		EXPDataResult.CharacterName = Character->BattleCharacterStats.CharacterName;
		EXPDataResult.InitialLV = Character->BattleCharacterStats.LV;
		EXPDataResult.InitialEXPValue = Character->BattleCharacterStats.EXP;
		EXPDataResult.InitialReqEXPValue = Character->BattleCharacterStats.ReqEXP;
		
		//check if character can gain exp
		for (auto e : Character->BattleCharacterStats.StatusEffects)
		{
			if (Character->EXPGainDisablingEffects.Contains(e))
			{
				//can't gain exp
				EXPDataResult.EXPValueResult = Character->BattleCharacterStats.EXP;
				EXPDataResult.ReqEXPValueResult = Character->BattleCharacterStats.ReqEXP;
				return EXPDataResult;
			}
		}

		//calculate how many levels are gained
		EXPDataResult.TotalEXPGain = PartyEXPGained;
		EXPDataResult.EXPValueResult = Character->BattleCharacterStats.EXP;
		EXPDataResult.ReqEXPValueResult = Character->BattleCharacterStats.ReqEXP;
		
		float EXPSum = EXPDataResult.TotalEXPGain + EXPDataResult.EXPValueResult;

		while (EXPSum >= EXPDataResult.ReqEXPValueResult)
		{
			EXPDataResult.LevelsGained++;
			
			EXPSum -= EXPDataResult.ReqEXPValueResult;
			
			EXPDataResult.ReqEXPValueResult = //find EXP requirement for next level
				FMath::Floor((1.5f + ((EXPDataResult.InitialLV + EXPDataResult.LevelsGained) * 0.005f)) * Character->BattleCharacterStats.ReqEXP);
			EXPDataResult.EXPValueResult = 0.f;
		}

		EXPDataResult.EXPValueResult = EXPSum;
	}

	return EXPDataResult;
}

void AFFYPlayerBattleManager::LevelUp(AFFYBattleCharacter* Character, FBattleEXPData& EXPData)
{
	if (Character)
	{
		Character->BattleCharacterStats.LV = EXPData.InitialLV + EXPData.LevelsGained;
		Character->BattleCharacterStats.EXP = EXPData.EXPValueResult;
		Character->BattleCharacterStats.ReqEXP = EXPData.ReqEXPValueResult;

		FBattleCharacterData* StatIncreases =
			LevelUpDataTableHandle.DataTable->FindRow<FBattleCharacterData>(Character->BattleCharacterStats.CharacterName, "", true);

		if (StatIncreases && EXPData.LevelsGained > 0) 
		{
			Character->BattleCharacterStats.MaxHP += StatIncreases->MaxHP * EXPData.LevelsGained;
			Character->BattleCharacterStats.MaxMP += StatIncreases->MaxMP * EXPData.LevelsGained;
			Character->BattleCharacterStats.Strength += StatIncreases->Strength * EXPData.LevelsGained;
			Character->BattleCharacterStats.Constitution += StatIncreases->Constitution * EXPData.LevelsGained;
			Character->BattleCharacterStats.Magic += StatIncreases->Magic * EXPData.LevelsGained;
			Character->BattleCharacterStats.Dexterity += StatIncreases->Dexterity * EXPData.LevelsGained;
			Character->BattleCharacterStats.Precision += StatIncreases->Precision * EXPData.LevelsGained;
			Character->BattleCharacterStats.Spirit += StatIncreases->Spirit * EXPData.LevelsGained;
			Character->BattleCharacterStats.Luck += StatIncreases->Luck * EXPData.LevelsGained;
		}
	}
}

void AFFYPlayerBattleManager::ResetBattleActiveState(EActionState NewActionState)
{
	if (NewActionState != EActionState::ACTING)
	{
		for (auto c : Party)
		{
			if (c)
			{
				c->OnActionStateChanged.RemoveDynamic(this, &AFFYPlayerBattleManager::ResetBattleActiveState);
				c->SetActiveState_Implementation(EActiveState::ACTIVE);
			}
		}
		for (auto e : Enemies)
		{
			if (e)
			{
				e->OnActionStateChanged.RemoveDynamic(this, &AFFYPlayerBattleManager::ResetBattleActiveState);
				e->SetActiveState_Implementation(EActiveState::ACTIVE);
			}
		}

		if (BattleWidget)
		{
			BattleWidget->ActiveModeSwitched(EActiveState::ACTIVE);
		}
	}
}

void AFFYPlayerBattleManager::SetBattleActiveState(AFFYBattleCharacter* Character)
{
	if (Character)
	{
		Character->OnActionStateChanged.AddUniqueDynamic(this, &AFFYPlayerBattleManager::ResetBattleActiveState);

		SetAllWaitMode(Character);
	}
}

void AFFYPlayerBattleManager::SetAllWaitMode(AFFYBattleCharacter* Character)
{
	for (auto c : Party)
	{
		if (c != Character)
		{
			c->SetActiveState_Implementation(EActiveState::WAIT);
		}
	}
	for (auto e : Enemies)
	{
		if (e != nullptr && e != Character)
		{
			e->SetActiveState_Implementation(EActiveState::WAIT);
		}
	}

	if (BattleWidget)
	{
		BattleWidget->ActiveModeSwitched(EActiveState::WAIT);
	}
}

void AFFYPlayerBattleManager::OnCameraActionSelected(AFFYBattleCharacter* Character,
                                                     FCameraActionContainer CameraActionContainer)
{
	if (Character && (CameraActionContainer.Priority > CurrentCameraPriority ||
		(CameraActionContainer.bCanOverride && CameraActionContainer.Priority == CurrentCameraPriority)))
	{
		StopOngoingCameraAction();
		CurrentCameraPriority = CameraActionContainer.Priority;
		OnCameraActionStarted(Character, CameraActionContainer);
	}
}


void AFFYPlayerBattleManager::UpdateEnemies(TArray<AFFYBattleCharacter*> LoadedEnemies)
{
		Enemies = LoadedEnemies;

		for (int i = 0; i < Enemies.Num(); i++)
		{
			if (BattleWidget)
			{
				BattleWidget->AddHUDSlot(Enemies[i]);
				//Enemies: Bind delegates
				Enemies[i]->OnCharacterDefeated.AddUniqueDynamic(this, &AFFYPlayerBattleManager::CheckWinCondition);
				Enemies[i]->OnWaitActionUsed.AddUniqueDynamic(this, &AFFYPlayerBattleManager::SetBattleActiveState);
				Enemies[i]->OnCameraActionSelected.AddUniqueDynamic(this, &AFFYPlayerBattleManager::OnCameraActionSelected);
			}
		}
}

void AFFYPlayerBattleManager::ActionUsed_Implementation(FName ActionName, bool bIsEnemy)
{
	ActionWidget->OnActionUsed(ActionName, bIsEnemy);
}

