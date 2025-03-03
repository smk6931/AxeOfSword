// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AxeOfSword : ModuleRules
{
	public AxeOfSword(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine",
			"InputCore", "EnhancedInput", "UMG", "GameplayAbilities", "GameplayTags", "GameplayTasks",
			"EngineCameras","NavigationSystem", "AIModule",  "MovieScene", "LevelSequence",
			"MovieSceneTracks", "Niagara" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
