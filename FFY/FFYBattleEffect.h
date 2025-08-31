// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FFYBattleEffect.generated.h"

class UNiagaraSystem;
class AFFYBattleCharacter;
class AFFYAction;


/*
 * Custom visual effect class that uses custom logic to determine how/when to trigger Action effects.
 */
UCLASS(Abstract)
class FFY_API AFFYBattleEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFFYBattleEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References", meta = (ExposeOnSpawn = true))
	AFFYAction* Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References", meta = (ExposeOnSpawn = true))
	AFFYBattleCharacter* User;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References", meta = (ExposeOnSpawn = true))
	TArray<AFFYBattleCharacter*> Targets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* SpawnedEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FName DefaultSocketName;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void FinishEffect();
};
