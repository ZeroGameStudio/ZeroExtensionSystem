// Copyright Zero Games. All Rights Reserved.

using UnrealBuildTool;

public class ZeroExtensionSystemEditor : ModuleRules
{
	public ZeroExtensionSystemEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange
		(
			new string[]
			{
			}
		);
		
		PrivateIncludePaths.AddRange
		(
			new string[]
			{
			}
		);
		
		PublicDependencyModuleNames.AddRange
		(
			new string[]
			{
				"Core",
			}
		);
		
		PrivateDependencyModuleNames.AddRange
		(
			new string[]
			{
				"CoreUObject",
				"Engine",
			}
		);
		
		DynamicallyLoadedModuleNames.AddRange
		(
			new string[]
			{
			}
		);
	}
}


