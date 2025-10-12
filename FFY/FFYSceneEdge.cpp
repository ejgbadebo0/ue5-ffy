// Source code implementation by Ephraim Gbadebo.


#include "FFYSceneEdge.h"

#include "FFYGameMode.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFFYSceneEdge::AFFYSceneEdge()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("SceneArrow"));
	SceneArrow->SetupAttachment(RootComponent);
	ChangeSceneBox = CreateDefaultSubobject<UBoxComponent>(FName("ChangeSceneBox"));
	ChangeSceneBox->SetupAttachment(SceneArrow);
	
}

// Called when the game starts or when spawned
void AFFYSceneEdge::BeginPlay()
{
	Super::BeginPlay();
	ChangeSceneBox->OnComponentBeginOverlap.AddDynamic(this, &AFFYSceneEdge::OnOverlapBegin);
	ChangeSceneBox->OnComponentEndOverlap.AddDynamic(this, &AFFYSceneEdge::OnOverlapEnd);
}

// Called every frame
void AFFYSceneEdge::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AFFYSceneEdge::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("BEGIN OVERLAP"));
}

void AFFYSceneEdge::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Green, TEXT("TRIGGERED"));
	AFFYCharacter* PlayerActor = Cast<AFFYCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (OtherActor && (OtherActor == PlayerActor)) 
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Green, TEXT("VALID COLLISION"));
		FVector PlayerDistance = PlayerActor->GetActorLocation() - GetActorLocation();
		PlayerDistance.Normalize();
	
		float TransferDot = FVector::DotProduct(SceneArrow->GetForwardVector(), FVector(PlayerDistance.X, PlayerDistance.Y, 0.0) );
		
		AFFYSceneCamera* TargetScene = (TransferDot >= 0) ? ToScene : FromScene;
		AFFYSceneCamera* OtherScene = (TargetScene == ToScene) ? FromScene : ToScene;
	
		// if this is a rotating/moving camera update the reference
		if (TargetScene->IsActorTickEnabled()) {
			TargetScene->UpdatePlayer(PlayerActor);
		}
		if (OtherScene->IsActorTickEnabled())
		{
			OtherScene->UpdatePlayer(nullptr);
		}

		AFFYGameMode* GameMode = Cast<AFFYGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->SetActiveViewTarget(TargetScene);
		}
		PlayerActor->SetActiveViewTarget(TargetScene);

		//GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Green, FString::Printf(TEXT("DOT: %d"), TransferDot));
		/*
		FString Vstring = PlayerDistance.ToString();
		FString Vstring2 = SceneArrow->GetForwardVector().ToString();
		DrawDebugLine(GetWorld(),
			SceneArrow->GetComponentLocation(),
			SceneArrow->GetComponentLocation() + (SceneArrow->GetForwardVector() * 50.0),
			FColor::Blue,
			false,
			50.0f,
			0U,
			3.f);
		GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Green, FString::Printf(TEXT("V1: %s, V2: %s"), *Vstring, *Vstring2));
		*/
		


		
		
	}
}

