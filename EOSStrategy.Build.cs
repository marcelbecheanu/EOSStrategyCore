// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EOSStrategy : ModuleRules
{
	public EOSStrategy(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "OnlineSubsystemEOS", "OnlineSubsystem", "OnlineSubsystemUtils" });
	}
}
