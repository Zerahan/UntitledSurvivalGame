// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UntitledSurvivalGameEditor : ModuleRules
{
	public UntitledSurvivalGameEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange( new string[] { "UntitledSurvivalGameEditor/Public" } );

		PrivateIncludePaths.AddRange( new string[] { "UntitledSurvivalGameEditor/Private" } );

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "UntitledSurvivalGame" } );
		
		PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "ComponentVisualizers" } );

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
