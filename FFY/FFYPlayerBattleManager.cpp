// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYPlayerBattleManager.h"

#include "FFYBattleGameMode.h"
#include "FFYBattleSpawnContainer.h"
#include "FFYBattleSpawnTransform.h"
#include "FFYGameInstance.h"
#include "Components/WrapBox.h"
#include "Widgets/FFYBattleWidget.h"

// Sets default values
AFFYPlayerBattleManager::AFFYPlayerBattleManager()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//initialize
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	MainCamera->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFFYPlayerBattleManager::BeginPlay()
{
	Super::BeginPlay();
	
	//Create widget and initialize variables
	UFFYGameInstance* GameInstance = Cast<UFFYGameInstance>(GetGameInstance());
	AFFYBattleSpawnContainer* SpawnContainer = Cast<AFFYBattleSpawnContainer>(UGameplayStatics::GetActorOfClass(GetWorld(), AFFYBattleSpawnContainer::StaticClass()));
	BattleWidget = CreateWidget<UFFYBattleWidget>(GetWorld(), MasterWidgetClass);

	if (GameInstance && SpawnContainer)
	{
		//hard cap at 4 active members or number of slots in battle arena
		for (int i = 0; i < 4 && i < GameInstance->GetParty().Num() && i < SpawnContainer->PartySpawns.Num(); i++)
		{
			AFFYBattleCharacter* PartySpawn = GetWorld()->SpawnActor<AFFYBattleCharacter>(GameInstance->GetParty()[i].PartyCharacterData.BattleCharacterClass, SpawnContainer->PartySpawns[i]->GetActorTransform());
			if (PartySpawn && BattleWidget)
			{
				Party.Emplace(PartySpawn);
				BattleWidget->AddHUDSlot(PartySpawn);
			}
		}
		BattleWidget->InitializeContextMenu(Party);
	}
	AFFYBattleGameMode* GameMode = Cast<AFFYBattleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->OnEnemiesLoaded.AddUniqueDynamic(this, &AFFYPlayerBattleManager::UpdateEnemies);
	GameMode->OnBattleStarted.AddUniqueDynamic(this, &AFFYPlayerBattleManager::OnStartBattle);

	
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

void AFFYPlayerBattleManager::UpdateEnemies(TArray<AFFYBattleCharacter*> LoadedEnemies)
{
		Enemies = LoadedEnemies;

		for (int i = 0; i < Enemies.Num(); i++)
		{
			if (BattleWidget)
			{
				BattleWidget->AddHUDSlot(Enemies[i]);
			}
		}
}

