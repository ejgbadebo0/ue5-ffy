// Source code implementation by Ephraim Gbadebo.


#include "FFYWidgetControlPawn.h"
//ext includes
#include <string>

#include "AudioMixerDevice.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FFYCharacter.h"
#include "FFYDataEnums.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFFYWidgetControlPawn::AFFYWidgetControlPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Init view camera
	WidgetCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("WidgetCamera"));
	WidgetCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	
}

// Called when the game starts or when spawned
void AFFYWidgetControlPawn::BeginPlay()
{
	Super::BeginPlay();


	AFFYCharacter* Character = Cast<AFFYCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		OwningCharacter = Character;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("PLAYER VALID"));
	}
	
	PauseWidget = CreateWidget<UFFYPauseWidget>(GetWorld(), MasterWidgetClass);
	if (PauseWidget)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("PAUSE Widget VALID"));
	}
	
}

void AFFYWidgetControlPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	//USED IN PLACE OF ON POSSESSED INTERFACE FUNCTION
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("ON POSSESSED TRIGGERED"));
	IFFYTransitions::OnPossessed_Implementation();
	if (PauseWidget)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController)
		{
			PlayerController->SetShowMouseCursor(true);
		}
		
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("ADDING TO VIEWPORT"));
		PauseWidget->AddToViewport();
	}
}

void AFFYWidgetControlPawn::Move(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, TEXT("MOVE"));
}

void AFFYWidgetControlPawn::Confirm(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CONFIRM"));
}

void AFFYWidgetControlPawn::Cancel(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CANCEL"));
	if (PauseWidget)
	{
		switch (PauseWidget->GetCurrentMenuMode())
		{
			default:
				break;
			case EMenuMode::NONE:
				PauseWidget->PreviousMenu();
				break;
			case EMenuMode::SELECTING:
				IFFYWidgetEvents* PauseWidgetInterface = Cast<IFFYWidgetEvents>(PauseWidget);
				if (PauseWidgetInterface)
				{
					PauseWidgetInterface->EndSelection_Implementation();
				}
				break;
		}
		
	}
}

void AFFYWidgetControlPawn::Cycle(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CYCLE - %f"), Value.GetMagnitude() ));
	if (PauseWidget)
	{
		IFFYWidgetEvents* PauseWidgetInterface = Cast<IFFYWidgetEvents>(PauseWidget->GetWidgetSwitcher()->GetActiveWidget());
		if (PauseWidgetInterface)
		{
			PauseWidgetInterface->CycleInputEvent_Implementation(FMath::Floor(Value.GetMagnitude()));
		}
		
	}
}

void AFFYWidgetControlPawn::Pause(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("PAUSE"));
	if (OwningCharacter)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("PLAYER VALID"));
		//
		IFFYTransitions* GameInstance = Cast<IFFYTransitions>(GetWorld()->GetGameInstance());
		if (GameInstance)
		{
			GameInstance->StartTransition_Implementation("Pause");
		}
		if (PauseWidget)
		{
			PauseWidget->RemoveFromParent();
		}
		GetController()->Possess(OwningCharacter);
	}
}

void AFFYWidgetControlPawn::SelectMode(const FInputActionValue& Value)
{
	if (PauseWidget)
	{
		IFFYWidgetEvents* Widget = Cast<IFFYWidgetEvents>(PauseWidget->GetWidgetSwitcher()->GetActiveWidget());
		if (Widget)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "SELECTMODE INPUT");
			Widget->SelectAllInputEvent_Implementation();
		}
	}
}

void AFFYWidgetControlPawn::OnPossessed_Implementation()
{
}

// Called every frame
void AFFYWidgetControlPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFFYWidgetControlPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Add Input Mapping Context
	
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
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFFYWidgetControlPawn::Move);
		

		// Custom Actions
		EnhancedInputComponent->BindAction(ConfirmAction, ETriggerEvent::Started, this, &AFFYWidgetControlPawn::Confirm);
		EnhancedInputComponent->BindAction(CancelAction, ETriggerEvent::Started, this, &AFFYWidgetControlPawn::Cancel);
		EnhancedInputComponent->BindAction(CycleAction, ETriggerEvent::Started, this, &AFFYWidgetControlPawn::Cycle);
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &AFFYWidgetControlPawn::Pause);
		EnhancedInputComponent->BindAction(SelectModeAction, ETriggerEvent::Started, this, &AFFYWidgetControlPawn::SelectMode);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

}

