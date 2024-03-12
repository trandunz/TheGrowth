// Copyright William Inman. All Rights Reserved.

using UnrealBuildTool;

public class TheGrowth : ModuleRules
{
	public TheGrowth(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
