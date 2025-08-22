// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FFYAnimationControls.h"
#include "FFYBattleCharacter.h"
#include "FFYDataEnums.h"
#include "FFYDataStructures.h"
#include "Animation/AnimInstanceProxy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FFYBattleCharacterAnimInstance.generated.h"

enum class EActionState : uint8;
class AFFYBattleCharacter;
class UMotionWarpingComponent;




USTRUCT(BlueprintType)
struct FFY_API FBattleCharacterAnimInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()

protected:

	virtual void Initialize(UAnimInstance* InAnimInstance) override;

	virtual void Update(float DeltaSeconds) override;
	
	bool CheckWeak()
	{
		bool LowHP = (OwningCharacter->BattleCharacterStats.HP/OwningCharacter->BattleCharacterStats.MaxHP) <= 0.25f;
		bool NegativeStatus = false;
		
		for (auto e : OwningCharacter->BattleCharacterStats.StatusEffects)
		{
			if (NegativeEffects.Contains(e))
			{
				NegativeStatus = true;
			}
		}
		
		return (LowHP || NegativeStatus);
	};

	//can't use editor to initialize
	void InitializeNegativeEffects()
	{
		NegativeEffects.Emplace(EStatusEffect::BURN);
		NegativeEffects.Emplace(EStatusEffect::POISON);
		NegativeEffects.Emplace(EStatusEffect::SILENCE);
		NegativeEffects.Emplace(EStatusEffect::BLIND);
		NegativeEffects.Emplace(EStatusEffect::PARALYSIS);
		NegativeEffects.Emplace(EStatusEffect::SLEEP);
		NegativeEffects.Emplace(EStatusEffect::BERSERK);
		NegativeEffects.Emplace(EStatusEffect::SHOCK);
		NegativeEffects.Emplace(EStatusEffect::SLEEP);
		NegativeEffects.Emplace(EStatusEffect::DRENCH);
		NegativeEffects.Emplace(EStatusEffect::FREEZE);
		NegativeEffects.Emplace(EStatusEffect::CONFUSE);
		NegativeEffects.Emplace(EStatusEffect::STAGGER);
	}

	UPROPERTY(Transient)
	TObjectPtr<UFFYBattleCharacterAnimInstance> AnimInstance = nullptr;

	UPROPERTY(Transient, BlueprintReadOnly, Category="References", meta=(AllowPrivateAccess=true))
	TObjectPtr<AFFYBattleCharacter> OwningCharacter = nullptr;

	UPROPERTY(Transient, BlueprintReadOnly, Category="References", meta=(AllowPrivateAccess=true))
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<EStatusEffect> NegativeEffects;

	UPROPERTY(Transient, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool bIsFalling = false;

	UPROPERTY(Transient, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool bIsDead = false;

	UPROPERTY(Transient, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool bIsWeak = false;
	
	UPROPERTY(Transient, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool bVictory = false;

	UPROPERTY(Transient, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	EActionState CurrentActionState = EActionState::IDLE;
	
};


/**
 * 
 */
UCLASS()
class FFY_API UFFYBattleCharacterAnimInstance : public UAnimInstance, public IFFYAnimationControls
{
	GENERATED_BODY()

protected:
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override
	{
		return &AnimInstanceProxy;
	}
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override {};


	UPROPERTY(Transient, BlueprintReadOnly, Category="Proxy", meta=(AllowPrivateAccess=true))
	FBattleCharacterAnimInstanceProxy AnimInstanceProxy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool bUpperBodyMontage = false;

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AFFYBattleCharacter* OwningCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCharacterMovementComponent* CharacterMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMotionWarpingComponent* MotionWarpingComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<FName, UAnimMontage*> ActionMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TObjectPtr<UAnimMontage>> UpperBodyMontages;

	UFUNCTION(BlueprintCallable)
	void EndMontage( UAnimMontage* Montage, bool bInterrupted);
	
	//INTERFACE:
//---------------
	virtual void PlayActionMontage_Implementation(FName Signature, bool bIsMultiTarget) override;
};
