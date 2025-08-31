// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FFYSceneCameraTriggerCollider.generated.h"

class AFFYSceneCamera;
class UBoxComponent;

UCLASS()
class FFY_API AFFYSceneCameraTriggerCollider : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFFYSceneCameraTriggerCollider();

	/** Scene Camera that settings will be modified on */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actors", meta = (AllowPrivateAccess = "true"))
	AFFYSceneCamera* SceneCamera;

	/** Collider used to change the active scene camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* TempEffectsCollsion;

	/** Boolean value to set the camera rotation to. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRotateValue = false;

	/** Boolean value to set the camera movement to. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMoveValue = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
