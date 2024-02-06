// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventorySystem.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_GAME_MODULE( FInventorySystem, InventorySystem );

void FInventorySystem::StartupModule()
{
	UE_LOG(LogTemp, Log, TEXT("StartupModule"));
}

void FInventorySystem::ShutdownModule()
{
	UE_LOG(LogTemp, Log, TEXT("ShutdownModule"));
}
 