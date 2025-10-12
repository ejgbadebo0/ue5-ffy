// Source code implementation by Ephraim Gbadebo.

#pragma once

#include "CoreMinimal.h"
#include "FFYDataStructures.h"
#include "UObject/Interface.h"
#include "FFYMusicEvents.generated.h"
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFFYMusicEvents : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FFY_API IFFYMusicEvents
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayMusic(FMusicData MusicData);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StopMusic(float FadeOutDuration = 0.2f);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FMusicData GetDefaultMusicData();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OverrideCurrentTrack(USoundCue* NewTrack, float LoopStartTime, float LoopEndTime);
	
};
