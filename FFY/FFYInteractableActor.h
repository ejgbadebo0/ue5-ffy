// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFYInteraction.h"
#include "GameFramework/Actor.h"
#include "FFYInteractableActor.generated.h"

class AFFYCharacter;
class UStaticMeshComponent;
class UCapsuleComponent;
class UWidgetComponent;
class USphereComponent;

UCLASS()
class FFY_API AFFYInteractableActor : public AActor, public IFFYInteraction
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//INTERFACE:
	virtual bool Interact_Implementation(AFFYInteractableActor* InteractActor) override;
	virtual void UpdateInteractActor_Implementation(AFFYInteractableActor* InteractActor) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool CheckInteraction(AFFYInteractableActor* InteractActor);
	virtual bool CheckInteraction_Implementation(AFFYInteractableActor* InteractActor);
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void ExecuteInteraction(AFFYCharacter* InteractingCharacter);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};

inline bool AFFYInteractableActor::CheckInteraction_Implementation(AFFYInteractableActor* InteractActor)
{
	return false;
}


