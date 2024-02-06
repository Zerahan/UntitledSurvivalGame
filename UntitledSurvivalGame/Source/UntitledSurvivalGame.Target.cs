// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class UntitledSurvivalGameTarget : TargetRules
{
	public UntitledSurvivalGameTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("UntitledSurvivalGame");
		ExtraModuleNames.Add("InventorySystem");
		//WindowsPlatform.PCHMemoryAllocationFactor = 2000;
	}
}
