// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VBAO_TPS : ModuleRules
{
	public VBAO_TPS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
