// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class UntitledSurvivalGameTarget : TargetRules
{
	public UntitledSurvivalGameTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("UntitledSurvivalGame");
		//WindowsPlatform.PCHMemoryAllocationFactor = 2000;
	}
}
