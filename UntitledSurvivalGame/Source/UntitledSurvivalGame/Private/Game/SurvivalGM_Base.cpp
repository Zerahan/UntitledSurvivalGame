// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SurvivalGM_Base.h"
#include "Game/SurvivalCharacter.h"

ASurvivalGM_Base::ASurvivalGM_Base()
{
	DefaultPawnClass = ASurvivalCharacter::StaticClass();
}
