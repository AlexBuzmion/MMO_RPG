// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MMO_Game : ModuleRules
{
	public MMO_Game(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "DedicatedServers" });
    }
}
