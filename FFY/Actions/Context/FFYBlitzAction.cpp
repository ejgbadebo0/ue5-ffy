// Source code implementation by Ephraim Gbadebo.


#include "FFYBlitzAction.h"

#include "FFY/FFYBattleCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

AFFYBlitzAction::AFFYBlitzAction()
{
	Label = "Blitz";
	Description = FText::FromString("Attack with a strike that hits multiple targets around user.");
	Type = 0;
	TargetType = ETargetType::SINGLE;
	ActionType = EActionType::NONE;
	bIsDefaultTargetEnemy = true;
	bQueuedAction = false;
	bIsPassive = false;
	ATBCost = 75;
	HPCost = 0;
	MPCost = 0;
	LPCost = 0;
	Rank = 1;
}

float AFFYBlitzAction::GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner)
{
	return FMath::Floor(Super::GetCalculatedActionValue(ActionOwner) * FMath::RandRange(1.f, 1.5f));
}

void AFFYBlitzAction::Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex)
{
	Super::Effect(ActionOwner, Target, HitIndex);

	TArray<FHitResult> HitResults;
	TArray<AActor*> HitActors;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn); // looking for Pawns within hit radius

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(500.f);
	FVector Origin = ActionOwner->GetActorLocation();

	bool bHit = ActionOwner->GetWorld()->SweepMultiByObjectType(
		HitResults,
		Origin,
		Origin,
		FQuat::Identity,
		ObjectQueryParams,
		CollisionShape
	);

	for (auto Hit : HitResults)
	{
		AActor* Actor = Hit.GetActor();
		if (Actor != nullptr && Actor != Target && Actor != ActionOwner && !HitActors.Contains(Actor)) //don't damage owner or target that was already hit
		{
			IFFYBattleEvents* BattleCharacter = Cast<IFFYBattleEvents>(Actor);
			if (BattleCharacter)
			{
				HitActors.Add(Actor);
				BattleCharacter->ReceiveDamage_Implementation(ActionOwner, this);
			}
		}
	}
}
