// Source code implementation by Ephraim Gbadebo.


#include "FFYSceneCameraTriggerCollider.h"
#include "Kismet/GameplayStatics.h"
#include "FFYSceneCamera.h"
#include "Components/BoxComponent.h"

// Sets default values
AFFYSceneCameraTriggerCollider::AFFYSceneCameraTriggerCollider()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TempEffectsCollsion = CreateDefaultSubobject<UBoxComponent>(FName("Temp Effects Collision"));
	TempEffectsCollsion->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AFFYSceneCameraTriggerCollider::BeginPlay()
{
	Super::BeginPlay();
	TempEffectsCollsion->OnComponentBeginOverlap.AddDynamic(this, &AFFYSceneCameraTriggerCollider::OnOverlapBegin);
	TempEffectsCollsion->OnComponentEndOverlap.AddDynamic(this, &AFFYSceneCameraTriggerCollider::OnOverlapEnd);

	//check if loading from another map if player is already in rotation area.
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (Character && TempEffectsCollsion->IsOverlappingActor(Character))
	{
		if (SceneCamera)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("COLLIDER: Begin")));
			SceneCamera->bIsRotating = true;
			//if (!SceneCamera->bIsRotating) { SceneCamera->ResetDefaultRotation(); }
		}
	}
	
}

// Called every frame
void AFFYSceneCameraTriggerCollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFFYSceneCameraTriggerCollider::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFFYCharacter* PlayerActor = Cast<AFFYCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (SceneCamera && OtherActor && (OtherActor == PlayerActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("COLLIDER: Begin")));
		SceneCamera->bIsRotating = true;
		//if (!SceneCamera->bIsRotating) { SceneCamera->ResetDefaultRotation(); }
	}

}

	// declare overlap end function

void AFFYSceneCameraTriggerCollider::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AFFYCharacter* PlayerActor = Cast<AFFYCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (SceneCamera && OtherActor && (OtherActor == PlayerActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("COLLIDER: End")));
		SceneCamera->bIsRotating = false;
		//if (!SceneCamera->bIsRotating) { SceneCamera->ResetDefaultRotation(); }
		//SceneCamera->ResetDefaultRotation();

	}
}

