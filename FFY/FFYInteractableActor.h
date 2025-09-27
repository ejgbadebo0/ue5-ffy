// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFYInteraction.h"
#include "FFYSaveDataEvents.h"
#include "GameFramework/Actor.h"
#include "FFYInteractableActor.generated.h"

class AFFYCharacter;
class UStaticMeshComponent;
class UCapsuleComponent;
class UWidgetComponent;
class USphereComponent;

UCLASS()
class FFY_API AFFYInteractableActor : public AActor, public IFFYInteraction, public IFFYSaveDataEvents
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFFYInteractableActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CapsuleComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UMeshComponent* Mesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	USphereComponent* InteractCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* InteractionWidgetComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	FName ActorID;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//OVERLAP:
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//========
	
	//INTERFACE:
	virtual bool Interact_Implementation(AFFYInteractableActor* InteractActor) override;
	virtual void UpdateInteractActor_Implementation(AFFYInteractableActor* InteractActor) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool CheckInteraction(AFFYInteractableActor* InteractActor);
	virtual bool CheckInteraction_Implementation(AFFYInteractableActor* InteractActor);
	
	UFUNCTION(BlueprintNativeEvent)
	void ExecuteInteraction(AFFYCharacter* InteractingCharacter);

	//BP EVENTS:
	UFUNCTION(BlueprintNativeEvent)
	void LoadSaveState(uint8 State);
};

inline void AFFYInteractableActor::LoadSaveState_Implementation(uint8 State)
{
}

inline bool AFFYInteractableActor::CheckInteraction_Implementation(AFFYInteractableActor* InteractActor)
{
	return false;
}


