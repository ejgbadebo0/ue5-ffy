// Copyright Epic Games, Inc. All Rights Reserved.

#include "FFYCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MotionWarpingComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FFYGameInstance.h"
#include "FFYGameMode.h"
#include "FFYInteractableActor.h"
#include "FFYWidgetControlPawn.h"
#include "InputActionValue.h"
#include "Chaos/Utilities.h"
#include "Components/WidgetComponent.h"
#include "Misc/OutputDeviceNull.h"
#include "Widgets/FFYMessageWidget.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AFFYCharacter

AFFYCharacter::AFFYCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	MainPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);


	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	//Custom Setup:
	//-------------
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));

	LedgeTraceOffset = GetCharacterMovement()->MaxStepHeight + 23.f;
	LastCameraRotation = FRotator::ZeroRotator;
}

//++++++++++++++++++++++++++++++++++++++++++++
//BEGIN PLAY
//==========================================
void AFFYCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	GameInstance = Cast<UFFYGameInstance>(GetGameInstance());
	
	if (GameInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("FOUND GAMEINSTANCE..."));
		bHasEncounters = GameInstance->bHasEncounters;
		GameInstance->OnWorldDamage.AddUniqueDynamic(this, &AFFYCharacter::WorldDamageEvent); 
	}
	
	if (bHasEncounters)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("ENCOUNTERS ON..."));
	}

	FActorSpawnParameters SpawnParams;
	if (WCPClass)
	{
		WidgetControlPawn = GetWorld()->SpawnActor<AFFYWidgetControlPawn>(WCPClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	}
	
	
}

void AFFYCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("REPOSSESSED CHARARCTER"));

	//reset mouse cursor for overworld
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->SetShowMouseCursor(false);
	}

	//get last scene camera
	if (CurrentSceneCamera)
	{
		SetActiveViewTarget(CurrentSceneCamera);
	}
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	
	//get game mode and reset camera for next tick
	GetWorld()->GetTimerManager().SetTimer(PMDTimerHandle, this, &AFFYCharacter::OnRepossess, 0.16f);
}

/*
 * Called on a timer to handle resetting view target variables
 */
void AFFYCharacter::OnRepossess()
{
	AFFYGameMode* GameMode = Cast<AFFYGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("CALLING INITCAMERA FUNC"));
		GameMode->InitSceneCameras();	
	}
	
	if (GameInstance)
	{
		FName CallbackName = GameInstance->GetConfirmCallbackName_Implementation();
		if (CallbackName != NAME_None)
		{
			UFunction* Function = this->FindFunction(CallbackName);
			if (Function)
			{
				this->ProcessEvent(Function, nullptr);
			}
		}
	}
	//timer handle that was used is reused from preserve movement.
	GetWorld()->GetTimerManager().ClearTimer(PMDTimerHandle);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFFYCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
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
		
		// Jumping
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFFYCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AFFYCharacter::OnStopMove);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFFYCharacter::Look);

		// Custom Actions
		EnhancedInputComponent->BindAction(ConfirmAction, ETriggerEvent::Started, this, &AFFYCharacter::Confirm);
		EnhancedInputComponent->BindAction(CancelAction, ETriggerEvent::Started, this, &AFFYCharacter::Cancel);
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &AFFYCharacter::Pause);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AFFYCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	//UE_LOG(LogTemp, Log, TEXT("FIRING"));
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, FString::Printf(TEXT("Vector: %s"), *VString) );

	if (Controller != nullptr && CharacterState == ECharacterState::NONE)
	{
		// find out which way is forward

		FString str = bPerserveMovementDirection ? "true" : "false";
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("perserve: %s"), *str));

		FRotator Rotation;
		if (CurrentSceneCamera)
		{
			Rotation = (bPerserveMovementDirection) ? LastCameraRotation : CurrentSceneCamera->GetActorRotation();
		}
		else {
			Rotation = Controller->GetControlRotation();
		} 
		
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get difference between input vectors this and last frame of movement
		FVector2D CompareVector = LastInputVector.GetRotated(Rotation.Yaw).GetSafeNormal();
		FVector2D PendingVector = MovementVector.GetRotated(Rotation.Yaw).GetSafeNormal();
		
		float InputDot = FVector2D::DotProduct(CompareVector, PendingVector);
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, FString::Printf(TEXT("DOT: %f"), InputDot) );
		if (InputDot < 0.75f) //stop preserving direction if enough of a change
		{
			StopPMDTimer();
		}
		LastInputVector = MovementVector;

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		FVector MV = GetMovementInputVector(ForwardDirection, RightDirection, MovementVector);
		bool TraceResult = ExecuteLedgeTrace(MV);

		// add movement 
		if (TraceResult) {
			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);

			if (bHasEncounters)
			{
				BattleCounter += GetWorld()->GetDeltaSeconds();
				GameInstance->EvaluateEncounter(BattleCounter);
			}
		}

	}
}

void AFFYCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFFYCharacter::Confirm(const FInputActionValue& Value)
{
	switch (CharacterState)
	{
	default:
		break;
	case ECharacterState::NONE:
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Player checks InteractActor"));
		if (CurrentInteractActor != nullptr)
		{
			IFFYInteraction* Interaction = Cast<IFFYInteraction>(CurrentInteractActor);
			bool Result = CurrentInteractActor->Interact_Implementation(nullptr);
			if (Result)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Player validates Interaction, Executing!"));
				CharacterState = ECharacterState::INTERACT;
				CurrentInteractActor->ExecuteInteraction(this);
			}
			break;
		}
		else
		{
			break;
		}
	case ECharacterState::INTERACT:
		break;
			
	case ECharacterState::MESSAGE:
		if (CurrentMessageWidget != nullptr)
		{
			CurrentMessageWidget->AdvanceMessage();
			break;
		}
		else
		{
			CharacterState = ECharacterState::NONE;
			break;
		}
	}
	
}

void AFFYCharacter::Cancel(const FInputActionValue& Value)
{
}

void AFFYCharacter::Pause(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("PAUSE STARTED"));
	if (WidgetControlPawn != nullptr && CharacterState == ECharacterState::NONE)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		if (GameInstance)
		{
			GameInstance->StartTransition_Implementation("Pause");
		}
		GetController()->Possess(WidgetControlPawn);
	}
}


////////////////////////////////////////////////////

bool AFFYCharacter::ExecuteLedgeTrace(FVector MovementVector) 
{
	FHitResult StepHitResult(ForceInit);
	FCollisionQueryParams CollisionTraceParams = FCollisionQueryParams(FName(TEXT("DefaultTrace")), false);

	FVector PlayerGroundLocation = GetActorLocation() + FVector(0.0, 0.0, -1.f * GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());
	FVector Start = PlayerGroundLocation + (MovementVector * GetCapsuleComponent()->GetUnscaledCapsuleRadius());
	FVector End = Start + FVector(0.0, 0.0, -1.f * LedgeTraceOffset);

	bool TraceResult = GetWorld()->LineTraceSingleByChannel(StepHitResult, Start, End, ECollisionChannel::ECC_Visibility, CollisionTraceParams);

	DrawDebugLine(GetWorld(),
		StepHitResult.TraceStart,
		(StepHitResult.bBlockingHit) ? StepHitResult.Location : StepHitResult.TraceEnd,
		(StepHitResult.bBlockingHit) ? FColor::Green : FColor::Red,
		false,
		0.0f,
		0U,
		1.f);

	return StepHitResult.bBlockingHit;
}


void AFFYCharacter::OnStopMove()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ONSTOP"));
	bPerserveMovementDirection = false;
}

void AFFYCharacter::StopPMDTimer()
{
	OnStopMove();
	GetWorld()->GetTimerManager().ClearTimer(PMDTimerHandle);
}

void AFFYCharacter::SetHasEncounters(bool Value)
{
	bHasEncounters = Value;
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("ENCOUNTERS UPDATED"));
}

FVector AFFYCharacter::GetMovementInputVector(FVector Forward, FVector Right, FVector2D Movement) {
	FVector SumVector = (Right * Movement.X) + (Forward * Movement.Y);
	return SumVector;
}

//INTERFACE:
//---------------
void AFFYCharacter::SetActiveViewTarget(AActor* Camera)
{

	// 
	bPerserveMovementDirection = true;
	if (CurrentSceneCamera) {
		LastCameraRotation = CurrentSceneCamera->GetActorRotation();
	}
	else {
		LastCameraRotation = Controller->GetControlRotation();
	}
	CurrentSceneCamera = Camera;

	if (Camera)
	{
		//FString str = Camera->GetActorLabel();
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Camera: %s"), *str));
		MainPlayerController->SetViewTarget(CurrentSceneCamera, FViewTargetTransitionParams());
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Camera: NULL")));

		MainPlayerController->SetViewTarget(this, FViewTargetTransitionParams());
	}

	GetWorld()->GetTimerManager().SetTimer(PMDTimerHandle, this, &AFFYCharacter::StopPMDTimer, 1.f);
}

void AFFYCharacter::UpdateInteractActor_Implementation(AFFYInteractableActor* InteractActor)
{
	IFFYInteraction::UpdateInteractActor_Implementation(InteractActor);

	if (InteractActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Player InteractActor updated!"));
		CurrentInteractActor = InteractActor;
	}
	else
	{
		CurrentInteractActor = nullptr;
	}
}

bool AFFYCharacter::Interact_Implementation(AFFYInteractableActor* InteractActor)
{
	return IFFYInteraction::Interact_Implementation(InteractActor);
}

