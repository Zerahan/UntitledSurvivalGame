// Copyright Epic Games, Inc. All Rights Reserved.

#include "UntitledSurvivalGameGameMode.h"
#include "UntitledSurvivalGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUntitledSurvivalGameGameMode::AUntitledSurvivalGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
