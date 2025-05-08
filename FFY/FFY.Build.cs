// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FFY : ModuleRules
{
	public FFY(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "Slate", "SlateCore", "MotionWarping", "NiagaraAnimNotifies", "Niagara"
    , "AnimGraphRuntime" });
    }
}
