// Zombieland by NikiYani. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ZombielandEditorTarget : TargetRules
{
	public ZombielandEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "Zombieland" } );
	}
}
