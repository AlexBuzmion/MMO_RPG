// Alex Buzmion II 2025

using UnrealBuildTool;
using System.Collections.Generic;

public class MMO_GameServerTarget : TargetRules {
    public MMO_GameServerTarget(TargetInfo Target) : base(Target) {
        Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
        ExtraModuleNames.Add("MMO_Game");
        ExtraModuleNames.Add("DedicatedServers");
    }
}
