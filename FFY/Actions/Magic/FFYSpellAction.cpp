// Source code implementation by Ephraim Gbadebo.


#include "FFYSpellAction.h"

#include "FFY/FFYBattleCharacter.h"
#include "Kismet/KismetMathLibrary.h"


void AFFYSpellAction::ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{

	if (Targets.Num() > 1)
	{
		FVector AveragedPosition = FVector::ZeroVector;
		for (int i = 0; i < Targets.Num(); i++)
		{
			AveragedPosition += Targets[i]->GetActorLocation();
		}
		AveragedPosition /= Targets.Num();
		ActionOwner->UpdateMotionWarpTransform("MagicCastWarp",
			ActionOwner->GetActorLocation(),
			FRotator(0.0f, UKismetMathLibrary::FindLookAtRotation(ActionOwner->GetActorLocation(), AveragedPosition).Yaw, 0.0f)
			);

		DebugLocations(ActionOwner->GetActorLocation(), FVector::ZeroVector, AveragedPosition, FVector::ZeroVector);
		
	}
	else
	{
		if (Targets[0] == ActionOwner)
		{
			ActionOwner->UpdateMotionWarpTransform("MagicCastWarp",
				ActionOwner->GetActorLocation(),
				ActionOwner->GetActorRotation()
				);
		}
		else
		{
			ActionOwner->UpdateMotionWarpTransform("MagicCastWarp",
				ActionOwner->GetActorLocation(),
				FRotator(0.0f, UKismetMathLibrary::FindLookAtRotation(ActionOwner->GetActorLocation(), Targets[0]->GetActorLocation()).Yaw, 0.0f)
				);
		}
	}

	ActionOwner->UpdateMotionWarpTransform("EndAttackWarp",
		ActionOwner->DefaultTransform.GetLocation(),
	 ActionOwner->DefaultTransform.GetRotation().Rotator()
	 );
	
	Super::ExecuteAction(ActionOwner, Targets);
}

void AFFYSpellAction::ExecuteAction(FBattleCharacterData& ActionOwner, TArray<FBattleCharacterData>& Targets)
{
	Super::ExecuteAction(ActionOwner, Targets);
}

void AFFYSpellAction::ExecuteAction(FBattleCharacterData& ActionOwner, FBattleCharacterData& Target)
{
	Super::ExecuteAction(ActionOwner, Target);
}

void AFFYSpellAction::DebugLocations(FVector Location1, FVector Normal1, FVector Location2, FVector Normal2, bool DrawSpheres)
{
	FHitResult StepHitResult(ForceInit);
	FCollisionQueryParams CollisionTraceParams = FCollisionQueryParams(FName(TEXT("DefaultTrace")), false);
	
	FVector Start = Location1 + Normal1;
	FVector End = Location2 + Normal2;

	bool TraceResult = GetWorld()->LineTraceSingleByChannel(StepHitResult, Start, End, ECollisionChannel::ECC_Visibility, CollisionTraceParams);

	DrawDebugLine(GetWorld(),
		StepHitResult.TraceStart,
		(StepHitResult.bBlockingHit) ? StepHitResult.Location : StepHitResult.TraceEnd,
		(StepHitResult.bBlockingHit) ? FColor::Green : FColor::Red,
		false,
		20.0f,
		0U,
		1.f);

	return;
}
