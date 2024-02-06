// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class InventorySystem : ModuleRules
{
	public InventorySystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange( new string[] { "InventorySystem/Public" } );

		PrivateIncludePaths.AddRange( new string[] { "InventorySystem/Private" } );

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "UntitledSurvivalGame" } );

		PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd" } );

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
