// Source code implementation by Ephraim Gbadebo.


#include "FFYAttackAction.h"
//ext includes
#include "FFY/FFYAnimationControls.h"
#include "FFY/FFYBattleCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AFFYAttackAction::AFFYAttackAction()
{
	Label = "Attack";
	Description = FText::FromString("Attack a target with equipped weapon.");
	Type = 0;
	TargetType = ETargetType::SINGLE;
	ActionType = EActionType::NONE;
	bIsDefaultTargetEnemy = true;
	bQueuedAction = true;
	bIsPassive = false;
	ATBCost = 25;
	HPCost = 0;
	MPCost = 0;
	LPCost = 0;
	Rank = 1;
}

float AFFYAttackAction::GetCalculatedActionValue(AFFYBattleCharacter* ActionOwner)
{
	return FMath::Floor(((ActionOwner->BattleCharacterStats.Strength/2) + ActionOwner->BattleCharacterStats.LV) * FMath::RandRange(0.8f, 1.2f));
}

void AFFYAttackAction::ExecuteAction(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	if (ActionOwner->ActorHasTag(FName("Enemy")))
	{
		ATBCost = 99.f;
	}
	
	EAttackType InAttackType = (ActionOwner->BattleCharacterStats.Equipment.Weapon.bIsEquipable) ? ActionOwner->BattleCharacterStats.Equipment.Weapon.EquipmentData.AttackType : EAttackType::MELEE;
	EDamageModifier InDamageModifier = (ActionOwner->BattleCharacterStats.Equipment.Weapon.bIsEquipable) ? ActionOwner->BattleCharacterStats.Equipment.Weapon.EquipmentData.DamageModifier : EDamageModifier::PHYSICAL;
	
	FVector EnemyFront = Targets[0]->GetActorLocation() + (Targets[0]->GetActorForwardVector() * 150.f) ;
	ActionOwner->UpdateMotionWarpTransform("TargetAttackWarp",
		EnemyFront,
		FRotator(0.0f, UKismetMathLibrary::FindLookAtRotation(ActionOwner->GetActorLocation(), Targets[0]->GetActorLocation()).Yaw, 0.0f)
		);
	ActionOwner->UpdateMotionWarpTransform("EndAttackWarp",
		ActionOwner->DefaultTransform.GetLocation(),
		 ActionOwner->DefaultTransform.GetRotation().Rotator()
		 );
	ActionOwner->UpdateDamageAttributes(
		GetCalculatedActionValue(ActionOwner),
		EStat::HP,
		InDamageModifier,
		ActionOwner->Infusion,
		TArray<EStatusEffect>(),
		TArray<EStatusEffect>(),
		0.0f,
		InAttackType);

	//DebugLocations(Targets[0]->GetActorLocation(), (Targets[0]->GetActorForwardVector() * 150.f), ActionOwner->GetActorLocation(), ActionOwner->GetActorForwardVector() * 150.f);
	
	Super::ExecuteAction(ActionOwner, Targets);
}

void AFFYAttackAction::ExecuteAction(FBattleCharacterData& ActionOwner, TArray<FBattleCharacterData>& Targets)
{
	Super::ExecuteAction(ActionOwner, Targets);
}

void AFFYAttackAction::Redirect(AFFYBattleCharacter* ActionOwner, TArray<AFFYBattleCharacter*> Targets)
{
	FVector EnemyFront = Targets[0]->GetActorLocation() + (Targets[0]->GetActorForwardVector() * 150.f) ;
	ActionOwner->UpdateMotionWarpTransform("TargetAttackWarp",
		EnemyFront,
		UKismetMathLibrary::FindLookAtRotation(ActionOwner->GetActorLocation(), Targets[0]->GetActorLocation())
		);
	
	IFFYAnimationControls* AnimInstance = Cast<IFFYAnimationControls>(ActionOwner->AnimInstance);
		
	if (AnimInstance != nullptr)
	{
		AnimInstance->PlayActionMontage_Implementation(Label, (Targets.Num() > 1));
	}
}

void AFFYAttackAction::DebugLocations(FVector Location1, FVector Normal1, FVector Location2, FVector Normal2, bool DrawSpheres)
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

	if (DrawSpheres)
	{
		DrawDebugSphere(
			GetWorld(),
			Location1,
			250.f,
			12,
			FColor::Green,
			true,
			-1,
			0U,
			1.f);

		DrawDebugSphere(
		GetWorld(),
		Location2,
		250.f,
		12,
		FColor::Purple,
		true,
		-1,
		0U,
		1.f);
	}

	return;
}

void AFFYAttackAction::Effect(AFFYBattleCharacter* ActionOwner, AFFYBattleCharacter* Target, int HitIndex)
{
	//Make sure to update again in case another action was chosen before
	EAttackType InAttackType = (ActionOwner->BattleCharacterStats.Equipment.Weapon.bIsEquipable) ? ActionOwner->BattleCharacterStats.Equipment.Weapon.EquipmentData.AttackType : EAttackType::MELEE;
	EDamageModifier InDamageModifier = (ActionOwner->BattleCharacterStats.Equipment.Weapon.bIsEquipable) ? ActionOwner->BattleCharacterStats.Equipment.Weapon.EquipmentData.DamageModifier : EDamageModifier::PHYSICAL;

	ActionOwner->UpdateDamageAttributes(
		GetCalculatedActionValue(ActionOwner),
		EStat::HP,
		InDamageModifier,
		ActionOwner->Infusion,
		TArray<EStatusEffect>(),
		TArray<EStatusEffect>(),
		0.0f,
		InAttackType);

	//Deal Damage
	ActionOwner->DamageDealtEvent(Target->ReceiveDamage_Implementation(ActionOwner, this));
}
