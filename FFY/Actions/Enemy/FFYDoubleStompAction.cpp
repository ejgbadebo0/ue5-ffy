// Source code implementation by Ephraim Gbadebo.


#include "FFYDoubleStompAction.h"

#include "FFY/FFYBattleCharacter.h"
#include "Kismet/KismetMathLibrary.h"

AFFYDoubleStompAction::AFFYDoubleStompAction()
{
	Label = "Double Stomp";
	Description = FText::FromString("Hits two random targets with physical damage.");
	Type = 0;
	TargetType = ETargetType::MULTI;
	ActionType = EActionType::NONE;
	bIsDefaultTargetEnemy = true;
	bQueuedAction = false;
	bIsPassive = false;
	ATBCost = 100;
	HPCost = 0;
	MPCost = 0;
	LPCost = 0;
	Rank = 1;
}

void AFFYDoubleStompAction::ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	
	FVector AveragedPosition = FVector::ZeroVector;
	for (int i = 0; i < Targets.Num(); i++)
	{
		AveragedPosition += Targets[i]->GetActorLocation();
	}
	AveragedPosition /= Targets.Num();
	//find directions
	FVector Forward = FRotator(0.0f, UKismetMathLibrary::FindLookAtRotation(ActionOwner->GetActorLocation(), Targets[0]->GetActorLocation()).Yaw, 0.0f).Vector();
	FVector LeftOffsetNormal = FVector::CrossProduct(Forward, FVector::UpVector);
	FVector RightOffsetNormal = FVector::CrossProduct(Forward, FVector::DownVector);
	
	//find actors in affected area
	TArray<FHitResult> LeftHitResults;
	TArray<FHitResult> RightHitResults;
	TArray<AActor*> HitActors;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn); // looking for Pawns within hit radius

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(425.f);
	FVector LeftSideOrigin = AveragedPosition + (LeftOffsetNormal * 350.f);
	FVector RightSideOrigin = AveragedPosition + (RightOffsetNormal * 350.f);

	TArray<AFFYBattleCharacter*> LeftActors;
	TArray<AFFYBattleCharacter*> RightActors;

	bool bLeftHit = ActionOwner->GetWorld()->SweepMultiByObjectType(
		LeftHitResults,
		LeftSideOrigin,
		LeftSideOrigin,
		FQuat::Identity,
		ObjectQueryParams,
		CollisionShape
	);

	bool bRightHit = ActionOwner->GetWorld()->SweepMultiByObjectType(
		RightHitResults,
		RightSideOrigin,
		RightSideOrigin,
		FQuat::Identity,
		ObjectQueryParams,
		CollisionShape
	);
	//get characters on left side
	for (auto Hit : LeftHitResults)
	{
		AActor* Actor = Hit.GetActor();
		if (Actor != nullptr && Actor != ActionOwner)
		{
			AFFYBattleCharacter* BattleCharacter = Cast<AFFYBattleCharacter>(Actor);
			if (BattleCharacter && !(BattleCharacter->GetIsDead_Implementation()))
			{
				LeftActors.Add(BattleCharacter);
			}
		}
	}
	//get characters on right side
	for (auto Hit : RightHitResults)
	{
		AActor* Actor = Hit.GetActor();
		if (Actor != nullptr && Actor != ActionOwner)
		{
			AFFYBattleCharacter* BattleCharacter = Cast<AFFYBattleCharacter>(Actor);
			if (BattleCharacter && !(BattleCharacter->GetIsDead_Implementation()))
			{
				RightActors.Add(BattleCharacter);
			}
		}
	}

	TArray<AFFYBattleCharacter*> FinalTargets;
	AFFYBattleCharacter* Left;
	AFFYBattleCharacter* Right;
    //Set left
	if (LeftActors.Num() > 0)
	{
		Left = LeftActors[FMath::RandRange(0, LeftActors.Num() - 1)];
	}
	else
	{
		for (auto Target : Targets)
		{
			if (!Target->GetIsDead_Implementation())
			{
				Left = Target;
				break;
			}
		}
	}
	//set right
	if (RightActors.Num() > 0)
	{
		Right = RightActors[FMath::RandRange(0, RightActors.Num() - 1)];
	}
	else
	{
		for (auto Target : Targets)
		{
			if (!Target->GetIsDead_Implementation())
			{
				Right = Target;
				break;
			}
		}
	}
	//add selected targets to list
	FinalTargets.Emplace(Left);
	FinalTargets.Emplace(Right);
	//DebugLocations(LeftSideOrigin, RightOffsetNormal, RightSideOrigin, LeftOffsetNormal, true);
	
	Super::ExecuteAction(ActionOwner, FinalTargets);
}

void AFFYDoubleStompAction::Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex)
{
	if (HitIndex > 0)
	{
		ActionOwner->UpdateDamageAttributes(
		GetCalculatedActionValue(ActionOwner),
		EStat::HP,
		EDamageModifier::PHYSICAL,
		ActionOwner->Infusion,
		TArray<EStatusEffect>(),
		TArray<EStatusEffect>(),
		0.0f,
		EAttackType::MELEE);

		//Deal Damage
		ActionOwner->DamageDealtEvent(ActionOwner->Targets[HitIndex]->ReceiveDamage_Implementation(ActionOwner, this));
	}
	else
	{
		ActionOwner->UpdateDamageAttributes(
		GetCalculatedActionValue(ActionOwner),
		EStat::HP,
		EDamageModifier::PHYSICAL,
		ActionOwner->Infusion,
		TArray<EStatusEffect>(),
		TArray<EStatusEffect>(),
		0.0f,
		EAttackType::MELEE);

		//Deal Damage
		ActionOwner->DamageDealtEvent(Target->ReceiveDamage_Implementation(ActionOwner, this));
	}

}
