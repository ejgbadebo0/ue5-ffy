// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//ext includes
#include "FFYSceneCamera.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"

#include "FFYSceneEdge.generated.h"

UCLASS()
class FFY_API AFFYSceneEdge : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFFYSceneEdge();

	/** Root component used to determine next scene. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arrow", meta=( AllowPrivateAccess = "true"))
	UArrowComponent* SceneArrow;

	/** Collider used to change the active scene camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta=( AllowPrivateAccess = "true"))
	UBoxComponent* ChangeSceneBox;

	/** Scene behind the arrow component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actors", meta=( AllowPrivateAccess = "true"))
	AFFYSceneCamera* FromScene;

	/** Scene in front of the arrow component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actors", meta = (AllowPrivateAccess = "true"))
	AFFYSceneCamera* ToScene;

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
