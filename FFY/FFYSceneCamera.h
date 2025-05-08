// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//ext includes
#include "Camera/CameraComponent.h"
#include "Components/SplineComponent.h"
#include "FFYCharacter.h"

#include "FFYSceneCamera.generated.h"

UCLASS()
class FFY_API AFFYSceneCamera : public AActor, public IFFYCameraControls
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFFYSceneCamera();

	/** The camera in the world that belongs to this actor */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* SceneCamera;

	/** Used for transitions. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USceneCaptureComponent2D* CaptureComponent;
	
	/** Spline for moving when bIsMoving is true, not set in hierarchy and will spawn in the center of the world. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	USplineComponent* SplineComponent;

	/** Reference to the player character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	AFFYCharacter* PlayerCharacter;

	/** Lerp start for moving camera angles, by default the starting position in the world. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTransform DefaultTransform;

	/** If true, LerpTransform variables will be used to move the camera's transform in relation to the player. */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool bIsMoving;

	/** If true, will rotate to center the player, can be set at runtime. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsRotating;

	/** The follow distance offset for the player when moving */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	float ViewOffset = 50.0f;

	/** The travel speed of the camera when following player movement */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	float LerpSpeed = 1.0f;

	FTimerHandle ResetTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	FVector GetLerpLocationAtDistanceAlongSpline();

	//need a function to update the player ref
	void UpdatePlayer(AFFYCharacter* Player);

	void ResetDefaultRotation();
	
	virtual void FrameCapture_Implementation() override;
};
