// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYSceneCamera.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFFYSceneCamera::AFFYSceneCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SceneCamera"));
	SceneCamera->SetupAttachment(RootComponent);

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));

	CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CaptureComponent"));
	CaptureComponent->SetupAttachment(SceneCamera);
	// only used as a view target
	SceneCamera->SetActive(false);
	

}

// Called when the game starts or when spawned
void AFFYSceneCamera::BeginPlay()
{
	Super::BeginPlay();

	DefaultTransform = GetActorTransform(); 
	this->SetActorTickEnabled((this->bIsMoving || this->bIsRotating) ? true : false);

	/*
	FString str =  this->IsActorTickEnabled() ? "true" : "false";
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("result: %s"), *str));
	*/
}

// Called every frame
void AFFYSceneCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 0.0, FColor::Cyan, TEXT("Is ACTIVE"));
	if (PlayerCharacter) {
		//if moving, lerp position on spline
		
		if (SplineComponent && bIsMoving)
		{
			FVector TargetLocation = GetLerpLocationAtDistanceAlongSpline();
			/*
			DrawDebugSphere(GetWorld(),
				TargetLocation,
				25.0f,
				12,
				FColor::Red,
				false,
				0.0f,
				0U,
				1.0f);
			*/
			FVector LerpLocation = FMath::Lerp(SceneCamera->GetComponentLocation(), TargetLocation, DeltaTime * LerpSpeed * 1.0f);
			SceneCamera->SetWorldLocation(LerpLocation, false, nullptr, ETeleportType::None);
		}

		//if rotating, lerp rotation to look at rotation for player
		if (bIsRotating)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.0, FColor::Purple, TEXT("ROTATING"));
			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(SceneCamera->GetComponentLocation(), PlayerCharacter->GetActorLocation());
			
			FRotator LerpRotation = FMath::RInterpTo(SceneCamera->GetComponentRotation(), TargetRotation, DeltaTime, 2);
			SceneCamera->SetWorldRotation(LerpRotation, false, nullptr, ETeleportType::None);
		}
		else {
			//if not rotating, check current rotation and lerp back if not default
			FRotator TargetRotation = FRotator(DefaultTransform.GetRotation());
			if (!SceneCamera->GetComponentRotation().Equals(TargetRotation, 0.1))
			{
				FRotator LerpRotation = FMath::RInterpTo(SceneCamera->GetComponentRotation(), TargetRotation, DeltaTime, 2);
				SceneCamera->SetWorldRotation(LerpRotation, false, nullptr, ETeleportType::None);
			}
		}
	}

}

FVector AFFYSceneCamera::GetLerpLocationAtDistanceAlongSpline()
{
	if (PlayerCharacter) 
	{

		float CheckOffset = 20.0f; //iteration increment value
		
		float MaxSplineDistance = SplineComponent->GetSplineLength(); //max spline length
		float ClosestDistance = 10000; //used to determine the best position in relation to player
		float TargetSplineLength = 0; //used to determine the location on spline to set camera
		
		for (float IterateDistance = 0; IterateDistance <= MaxSplineDistance; IterateDistance += CheckOffset)
		{
			FVector CheckLocation = SplineComponent->GetLocationAtDistanceAlongSpline(IterateDistance, ESplineCoordinateSpace::World);
			float CheckDistance = FVector::Distance(CheckLocation, PlayerCharacter->GetActorLocation());
			if (CheckDistance < ClosestDistance)
			{
				ClosestDistance = CheckDistance;
				TargetSplineLength = IterateDistance;
			}

			//make sure to do the final check at the very end of the spline
			
			if (IterateDistance > (MaxSplineDistance - CheckOffset) && IterateDistance < MaxSplineDistance )
			{
				IterateDistance = MaxSplineDistance - CheckOffset;
			}
			
		}

		GEngine->AddOnScreenDebugMessage(-1, 0.0, FColor::Green, FString::Printf(TEXT("SplineDist: %f"), ClosestDistance));

		//take view offset away from final distance and limit to spline params
		TargetSplineLength = FMath::Clamp((TargetSplineLength - ViewOffset), 0, MaxSplineDistance);

		FVector ClosestLocation = SplineComponent->GetLocationAtDistanceAlongSpline(TargetSplineLength, ESplineCoordinateSpace::World);
		return ClosestLocation;
	}

	return DefaultTransform.GetLocation();
}

void AFFYSceneCamera::UpdatePlayer(AFFYCharacter* Player)
{
	if (Player)
	{
		PlayerCharacter = Player;
	}
	else 
	{
		PlayerCharacter = nullptr;
	}
}

//NOT IN USE:
//-------------------
void AFFYSceneCamera::ResetDefaultRotation()
{
	//if nearly at the default rotation, set default rotation and clear timer
	float DeltaSeconds = GetWorld()->GetDeltaSeconds();
	FRotator TargetRotation = FRotator(DefaultTransform.GetRotation());
	bIsRotating = false;

	if (SceneCamera->GetComponentRotation().Equals( TargetRotation, 0.1 ))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("CAMERA: RESET COMPLETE")));
		SceneCamera->SetWorldRotation(DefaultTransform.GetRotation(), false, nullptr, ETeleportType::None);
		GetWorld()->GetTimerManager().ClearTimer(ResetTimerHandle);
	}
	else { //set timer for recurse and lerp rotation
		GetWorld()->GetTimerManager().SetTimer(ResetTimerHandle, this, &AFFYSceneCamera::ResetDefaultRotation, DeltaSeconds);
		FRotator LerpRotation = FMath::RInterpTo(SceneCamera->GetComponentRotation(), TargetRotation, DeltaSeconds * 2, 2.f);
		SceneCamera->SetWorldRotation(LerpRotation, false, nullptr, ETeleportType::None);
	}

}

void AFFYSceneCamera::FrameCapture_Implementation()
{
	if (CaptureComponent)
	{
		CaptureComponent->CaptureScene();
	}
}

//----------------
