// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectLiminal : ModuleRules
{
	public ProjectLiminal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "UMG", "Niagara"});

        PrivateDependencyModuleNames.AddRange(new string[] { "MovieScene", "LevelSequence" });
    }
}
