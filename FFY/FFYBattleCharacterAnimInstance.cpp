// Source code implementation by Ephraim Gbadebo.


#include "FFYBattleCharacterAnimInstance.h"
#include "FFYBattleCharacter.h"

//PROXY:
void FBattleCharacterAnimInstanceProxy::Initialize(UAnimInstance* InAnimInstance)
{
	FAnimInstanceProxy::Initialize(InAnimInstance);

	if (!InAnimInstance)
	{
		return;
	}
	else
	{
		AnimInstance = Cast<UFFYBattleCharacterAnimInstance>(InAnimInstance);
		if (AnimInstance)
		{
			OwningCharacter =  Cast<AFFYBattleCharacter>(AnimInstance->GetOwningActor());
			if (OwningCharacter)
			{
				CharacterMovementComponent = OwningCharacter->GetCharacterMovement();
				InitializeNegativeEffects();
				//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Silver, "PROXY INITIALIZED!!!");
			}
		}
	}
}

void FBattleCharacterAnimInstanceProxy::Update(float DeltaSeconds)
{
	FAnimInstanceProxy::Update(DeltaSeconds);

	if (CharacterMovementComponent)
	{
		bIsFalling = CharacterMovementComponent->IsFalling();
		bIsDead = OwningCharacter->GetIsDead_Implementation();
		bIsWeak = CheckWeak();
		bVictory = OwningCharacter->bVictoryState;
		CurrentActionState = OwningCharacter->ActionState;
	}
}


//=============================================================
void UFFYBattleCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningCharacter = Cast<AFFYBattleCharacter>(GetOwningActor());
	
	if (OwningCharacter) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("ANIMINSTANCE: Cast Successful")));
		CharacterMovementComponent = OwningCharacter->GetCharacterMovement();
		MotionWarpingComponent = OwningCharacter->MotionWarpingComponent;
	}
}

void UFFYBattleCharacterAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	if (OwningCharacter)
	{
		//do stuff
	}
}

void UFFYBattleCharacterAnimInstance::EndMontage(UAnimMontage* Montage, bool bInterrupted)
{
	OnMontageEnded.RemoveDynamic(this, &UFFYBattleCharacterAnimInstance::EndMontage);

	bUpperBodyMontage = false;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "ENDMONTAGE EVENT CALLED");
}

void UFFYBattleCharacterAnimInstance::PlayActionMontage_Implementation(FName Signature, bool bIsMultiTarget)
{
	//CUSTOM BEHAVIOR:

	if (Signature == FName("Attack"))
	{
		if (OwningCharacter->Targets[0])
		{
			float Distance = (OwningCharacter->GetActorLocation() - OwningCharacter->Targets[0]->GetActorLocation()).Length();
			if (Distance <= 550.f)
			{
				Signature = "AttackNear";
			}
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("DISTANCE: %f"), Distance));
		}
	}
	//==============
	UAnimMontage** ToPlay;
	if (bIsMultiTarget)
	{
		FName MultiTargetSignature = FName(Signature.ToString() + "All");
		ToPlay = ActionMontages.Find(MultiTargetSignature);
		if (!ToPlay)
		{
			ToPlay = ActionMontages.Find(Signature);
		}
	}
	else
	{
		ToPlay = ActionMontages.Find(Signature);
	}
	if (ToPlay)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("ANIMINSTANCE: Valid Montage")));
		if (OwningCharacter)
		{
			if (UpperBodyMontages.Contains(*ToPlay))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("ANIMINSTANCE: Playing on UPPERBODY")));
				bUpperBodyMontage = true;
			}
			OwningCharacter->PlayAnimMontage(*ToPlay, 1, NAME_None);
			OnMontageEnded.AddUniqueDynamic(this, &UFFYBattleCharacterAnimInstance::EndMontage);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("ANIMINSTANCE: INVALID Montage")));
	}
}
