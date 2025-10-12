// Source code implementation by Ephraim Gbadebo.


#include "FFYStatusEffectComponent.h"

void UFFYStatusEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
