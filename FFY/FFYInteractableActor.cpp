// Fill out your copyright notice in the Description page of Project Settings.


#include "FFYInteractableActor.h"

#include "FFYCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/ContentEncryptionConfig.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFFYInteractableActor::AFFYInteractableActor() 
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(FName("Capsule"));
	RootComponent = CapsuleComponent;

	Mesh = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	if (Mesh)
	{
		Mesh->SetupAttachment(CapsuleComponent);
	}

	InteractCollision = CreateDefaultSubobject<USphereComponent>(TEXT("InteractCollision"));
	InteractCollision->SetupAttachment(GetRootComponent());
	
	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	InteractionWidgetComponent->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AFFYInteractableActor::BeginPlay()
{
	Super::BeginPlay();

	InteractCollision->OnComponentBeginOverlap.AddDynamic(this, &AFFYInteractableActor::OnOverlapBegin);
	InteractCollision->OnComponentEndOverlap.AddDynamic(this, &AFFYInteractableActor::OnOverlapEnd);
}

//INTERFACE:

bool AFFYInteractableActor::Interact_Implementation(AFFYInteractableActor* InteractActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Base Interaction implementation, Returning false"));
	return CheckInteraction(InteractActor);
}

void AFFYInteractableActor::UpdateInteractActor_Implementation(AFFYInteractableActor* InteractActor)
{
	IFFYInteraction::UpdateInteractActor_Implementation(InteractActor);
}




//-------------
// Called every frame
void AFFYInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*
 * Called when Interact returns true, executes custom interaction logic. 
 */
void AFFYInteractableActor::ExecuteInteraction_Implementation(AFFYCharacter* InteractingCharacter)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Interact Successful!");
	InteractionWidgetComponent->SetVisibility(false);
}


void AFFYInteractableActor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                           class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Player overlaps"));
	if (OtherActor == Character) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Player is valid "));
		IFFYInteraction* InteractingCharacter = Cast<IFFYInteraction>(Character);
		if (InteractingCharacter)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Calling update on player..."));
			InteractingCharacter->UpdateInteractActor_Implementation(this);
			InteractionWidgetComponent->SetVisibility(true);
		}
	}
}

void AFFYInteractableActor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (OtherActor == Character)
	{
		IFFYInteraction* InteractingCharacter = Cast<IFFYInteraction>(Character);
		if (InteractingCharacter)
		{
			InteractingCharacter->UpdateInteractActor_Implementation(nullptr);
			InteractionWidgetComponent->SetVisibility(false);
		}
	}
	
}

