// Zombieland by NikiYani. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ZombielandTarget : TargetRules
{
	public ZombielandTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "Zombieland" } );
	}
}
