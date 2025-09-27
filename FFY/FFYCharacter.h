// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
//ext includes
#include "FFYBattleEvents.h"
#include "FFYCameraControls.h"
#include "FFYInteraction.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TimerHandle.h"
#include "FFYCharacter.generated.h"

class UFFYMessageWidget;
class AFFYInteractableActor;
class AFFYWidgetControlPawn;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UMotionWarpingComponent;
class UFFYGameInstance;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	NONE UMETA(DisplayName = "None"),
	INTERACT UMETA(DisplayName = "Interact"),
	MESSAGE UMETA(DisplayName = "Message"),
	E_MAX UMETA(Hidden)
};

UCLASS(config=Game)
class AFFYCharacter : public ACharacter, public IFFYCameraControls, public IFFYInteraction, public IFFYBattleEvents
{
	GENERATED_BODY()


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller", meta = (AllowPrivateAccess = "true"))
	APlayerController* MainPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller", meta = (AllowPrivateAccess = "true"))
	AFFYWidgetControlPawn* WidgetControlPawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controller", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AFFYWidgetControlPawn> WCPClass;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UMotionWarpingComponent* MotionWarpingComponent;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	//AXIS:
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	//ACTION:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ConfirmAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CancelAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	FVector2D LastInputVector = FVector2D::ZeroVector;

public:
	AFFYCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Confirm(const FInputActionValue& Value);
	
	void Cancel(const FInputActionValue& Value);
	
	void Pause(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent)
	void WorldDamageEvent(bool bIsPoison);
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void PossessedBy(AController* NewController) override;
	
	//TIMER FUNCTIONS:
	void OnRepossess();
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable, Category = "Collision")
	bool ExecuteLedgeTrace(FVector MovementVector);

	//FUNCTIONS:
	//--------------
	
	UFUNCTION()
	void OnStopMove();

	/** Helper function for moving in relation to current camera. */
	UFUNCTION(BlueprintCallable, Category = "Collision")
	FVector GetMovementInputVector(FVector Forward, FVector Right, FVector2D Movement);



	//VARIABLES
	//---------------

	/** Used for world camera objects */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	AActor* CurrentSceneCamera;

	/** Height trace value for preventing stepping over cliffs */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float LedgeTraceOffset;

	/** Used to perserve movement direction when changing cameras */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bPerserveMovementDirection = false;

	/** Used when changing scenes to preserve move axis */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	FRotator LastCameraRotation;

	/** Used for manually stopping movement preserve after changing scene */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	FTimerHandle PMDTimerHandle;

	void StopPMDTimer();

	void SetHasEncounters(bool Value);

	UFFYGameInstance* GameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	ECharacterState CharacterState = ECharacterState::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actors")
	AFFYInteractableActor* CurrentInteractActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actors")
	UFFYMessageWidget* CurrentMessageWidget;

	
	
	//INTERFACE:
	//---------------
	UFUNCTION(BlueprintCallable)
	virtual void SetActiveViewTarget(AActor* Camera) override;

	virtual void UpdateInteractActor_Implementation(AFFYInteractableActor* InteractActor) override;
	virtual bool Interact_Implementation(AFFYInteractableActor* InteractActor) override;

private:
	float BattleCounter = 0;

	bool bHasEncounters = false;


};

