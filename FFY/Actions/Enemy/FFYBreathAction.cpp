// Source code implementation by Ephraim Gbadebo.


#include "FFYBreathAction.h"

#include "FFY/FFYAnimationControls.h"
#include "FFY/FFYBattleCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AFFYBreathAction::AFFYBreathAction()
{
	Label = "Breath";
	Description = FText::FromString("A flaming breath attack that hits all party members.");
	Type = 0;
	TargetType = ETargetType::MULTI;
	ActionType = EActionType::MAGIC;
	bIsDefaultTargetEnemy = true;
	bQueuedAction = false;
	bIsPassive = false;
	ATBCost = 100;
	HPCost = 0;
	MPCost = 0;
	LPCost = 0;
	Rank = 3;
}

float AFFYBreathAction::GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner)
{
	return FMath::Floor(Rank * ((FMath::Square(ActionOwner->BattleCharacterStats.Magic))/6 + Rank)/4 );
}

void AFFYBreathAction::ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	if (CanUse(ActionOwner, Targets.Num()))
	{
		
		if (!CanExecute(ActionOwner) || (ActionOwner->ActionState == EActionState::ACTING) || (ActionOwner->ActiveState == EActiveState::WAIT)) //if not currently in a state where character can expend ATB
		{
			return;
		}
		Consume(ActionOwner, Targets.Num());

		//Set variables for delayed effects separate from animation
		ActionOwner->ActionContainer->LastAction = this;
		ActionOwner->Targets = Targets;

		//Play animation tied to this action
		IFFYAnimationControls* AnimInstance = Cast<IFFYAnimationControls>(ActionOwner->AnimInstance);
		
		if (AnimInstance != nullptr)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Silver, "ANIM INSTANCE VALID: Play Montage");
			if (ActionOwner->GetCharacterMovement() != nullptr)
			{
				bool Flying = ActionOwner->GetCharacterMovement()->IsFlying();

				if (Flying)
				{
					if (ActionOwner->MotionWarpingComponent != nullptr)
					{
						//calculate flight path for air version of move
						FVector AveragedPosition = FVector::ZeroVector;
						for (int i = 0; i < Targets.Num(); i++)
						{
							AveragedPosition += Targets[i]->GetActorLocation();
						}
						AveragedPosition /= Targets.Num();
						FVector Forward = FRotator(0.0f, UKismetMathLibrary::FindLookAtRotation(ActionOwner->GetActorLocation(), AveragedPosition).Yaw, 0.0f).GetInverse().Vector();
						FVector HorizontalOffsetNormal = FVector::CrossProduct(Forward, FVector::UpVector);
						FVector StartFlightDestination = AveragedPosition + (FVector::UpVector * ActionOwner->FlightHeight) + (HorizontalOffsetNormal *  ActionOwner->FlightHeight * 2.f);
						FVector EndFlightDestination = AveragedPosition + (FVector::UpVector * ActionOwner->FlightHeight) + (HorizontalOffsetNormal * -1.f * ActionOwner->FlightHeight);
						FRotator StartFlightRotation = FRotator(0.0f, UKismetMathLibrary::FindLookAtRotation(StartFlightDestination, AveragedPosition).Yaw, 0.0f);
						FRotator ReturnFlightRotation = FRotator(0.0f, UKismetMathLibrary::FindLookAtRotation(EndFlightDestination, ActionOwner->DefaultTransform.GetLocation()).Yaw, 0.0f);
						//update warp points
						
						ActionOwner->MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(FName("BreathStartWarp"), StartFlightDestination, StartFlightRotation);
						ActionOwner->MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(FName("BreathEndWarp"), EndFlightDestination, StartFlightRotation);
						ActionOwner->MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(FName("ReturnWarp"), ActionOwner->DefaultTransform.GetLocation(), ReturnFlightRotation);
						ActionOwner->MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform(FName("EndAttackWarp"), ActionOwner->DefaultTransform);
					}
				}

				AnimInstance->PlayActionMontage_Implementation((Flying) ? FName("BreathAir") : Label, (Targets.Num() > 1));

				ActionOwner->ActionUsed_Implementation(GetMenuLabel(), ActionOwner->ActorHasTag(FName("Enemy")));
			}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Silver, "FAILED TO EXECUTE: Resources");
		if (ActionOwner->ActorHasTag(FName("Enemy")))
		{
			ActionOwner->ATB -= 10.f;
		}
	}
}

void AFFYBreathAction::Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex)
{
	ActionOwner->UpdateDamageAttributes(
		GetCalculatedActionValue(ActionOwner),
		EStat::HP,
		EDamageModifier::MAGIC,
		EElement::FIRE,
		TArray<EStatusEffect>(),
		TArray<EStatusEffect>(),
		0.0f,
		EAttackType::RANGED);

	//Deal Damage
	Target->ReceiveDamage_Implementation(ActionOwner, this);
}
