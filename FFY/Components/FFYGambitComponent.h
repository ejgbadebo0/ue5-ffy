// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FFYGambitComponent.generated.h"

class AFFYBattleCharacter;



UENUM (BlueprintType)
enum class ETargetClass : uint8
{
	NONE UMETA(DisplayName = "None"),
	SELF UMETA(DisplayName = "Self"),
	PARTY UMETA(DisplayName = "Party"),
	ENEMY UMETA(DisplayName = "Enemy"),
	RANDOM UMETA(DisplayName = "Random"),
	E_MAX UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FFY_API FGambit
{
	GENERATED_BODY()

	//in case where this gambit is selected, whether to evaluate again or pass to next in list
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EvaluateOnce = false;

	//if non-zero positive number, target's HP ratio being below this value will return true
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HPRatioLimit = 0.f;

	//if non-zero positive number, target's HP ratio being above this value will return true
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HPRatioFloor = 0.f;

	//if non-zero positive number, self's MP ratio being below this value will return true
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MPRatioLimit = 0.f;

	//if non-zero positive number, self's MP ratio being above this value will return true
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MPRatioFloor = 0.f;

	//if non-zero positive number, number of actors currently tagged as "Enemy" will return true
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 NumEnemies = 0;

	//if non-zero positive number, number of actors currently tagged as "Party" will return true
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 NumAllies = 0;

	//if true and action selected can TargetAll, will use TargetAll
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTargetAll = false;

	//if true, characters with KO status cannot be targeted
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFilterDead = true;

	//what type of target to perform action on
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETargetClass TargetClass = ETargetClass::NONE;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGambitSelected, FName, ActionName, FGambit, Gambit);
/*
 * Component for evaluating and executing Gambits for AI-controlled enemies/party members
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FFY_API UFFYGambitComponent : public UActorComponent
{
	GENERATED_BODY()

	//used to determine whether to evaluate gambits(tick) or not
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Default", meta=(AllowPrivateAccess=true))
	bool bGambitsActive;

	//used in cases where we don't want to use the same gambit twice
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Default", meta=(AllowPrivateAccess=true))
	FName LastSelection;

public:	
	// Sets default values for this component's properties
	UFFYGambitComponent();

	//DELEGATES:
	FOnGambitSelected OnGambitSelected;
	//========

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//reference to owning character
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	AFFYBattleCharacter* OwnerCharacter;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetGambitsActive(bool bActive)
	{
		bGambitsActive = bActive;
		PrimaryComponentTick.bCanEverTick = bGambitsActive;
	};

	UFUNCTION(BlueprintCallable)
	bool GetGambitsActive() const
	{
		return bGambitsActive;
	};
	
	//list of gambits that controlled member will evaluate
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Default", meta=(AllowPrivateAccess=true))
	TMap<FName, FGambit> Gambits;


	//===============
	
	UFUNCTION(BlueprintCallable, Category="Default", meta=(AllowPrivateAccess=true))
	void EvaluateGambits(float ATBValue);

	//helper function, return whether target meets all Gambit conditions
	UFUNCTION(BlueprintCallable, Category="Default", meta=(AllowPrivateAccess=true))
	bool ValidGambitConditions(AFFYBattleCharacter* Target, FGambit& Gambit);
};
