// Copyright Epic Games, Inc. All Rights Reserved.

#include "UntitledSurvivalGameEditor.h"
#include "UnrealEd.h"
#include "AlignmentHelperVisualizer.h"
#include "Dev/MapGeneration/AlignmentHelperComponent.h"
//#include "Modules/ModuleManager.h"

IMPLEMENT_GAME_MODULE( FUntitledSurvivalGameEditor, UntitledSurvivalGameEditor );

void FUntitledSurvivalGameEditor::StartupModule()
{
	UE_LOG(LogTemp, Log, TEXT("StartupModule"));
	if (GUnrealEd) {
		TSharedPtr<FAlignmentHelperVisualizer> Visualizer = MakeShareable(new FAlignmentHelperVisualizer());
		GUnrealEd->RegisterComponentVisualizer(UAlignmentHelperComponent::StaticClass()->GetFName(), Visualizer);
		Visualizer->OnRegister();
	}
}

void FUntitledSurvivalGameEditor::ShutdownModule()
{
	UE_LOG(LogTemp, Log, TEXT("ShutdownModule"));
	if (GUnrealEd) {
		GUnrealEd->UnregisterComponentVisualizer(UAlignmentHelperComponent::StaticClass()->GetFName());
	}
}
 