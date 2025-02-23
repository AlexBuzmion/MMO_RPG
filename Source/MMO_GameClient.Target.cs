// Alex Buzmion II 2025

using UnrealBuildTool;
using System.Collections.Generic;

public class MMO_GameClientTarget : TargetRules {
	public MMO_GameClientTarget(TargetInfo Target) : base(Target) {
		Type = TargetType.Client;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		ExtraModuleNames.Add("MMO_Game");
		ExtraModuleNames.Add("DedicatedServers");
	}
}