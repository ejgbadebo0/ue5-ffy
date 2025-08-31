// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFYTransitions.h"
#include "GameFramework/Pawn.h"
#include "Widgets/FFYPauseWidget.h"
#include "FFYWidgetControlPawn.generated.h"

class AFFYCharacter;
class UCameraComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS()
class FFY_API AFFYWidgetControlPawn : public APawn, public IFFYTransitions
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFFYWidgetControlPawn();

	//WIDGETS:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UFFYMasterWidget> MasterWidgetClass;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	UFFYPauseWidget* PauseWidget;
	//

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	UFFYMenuWidget* ActiveMenuContext;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	//AXIS:
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	

	//ACTION:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ConfirmAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CancelAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CycleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SelectModeAction;

	//COMPONENT:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	AFFYCharacter* OwningCharacter;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* WidgetCamera;
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	//INPUT FUNCTIONS
	void Move(const FInputActionValue& Value);

	void Confirm(const FInputActionValue& Value);
	
	void Cancel(const FInputActionValue& Value);

	void Cycle(const FInputActionValue& Value);
	
	void Pause(const FInputActionValue& Value);

	void SelectMode(const FInputActionValue& Value);

	//----------
	
	virtual void OnPossessed_Implementation() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
