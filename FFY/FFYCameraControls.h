// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "UObject/Interface.h"
#include "FFYCameraControls.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UFFYCameraControls : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FFY_API IFFYCameraControls
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void SetActiveCamera(UCameraComponent* Camera);

	virtual void SetActiveViewTarget(AActor* Camera);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FrameCapture();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	class AFFYSceneCamera* GetSceneCameraActor();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateInitialTarget(AFFYCharacter* Player);
};
