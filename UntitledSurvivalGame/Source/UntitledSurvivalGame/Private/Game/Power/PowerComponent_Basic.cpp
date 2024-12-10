// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Power/PowerComponent_Basic.h"

// Sets default values for this component's properties
UPowerComponent_Basic::UPowerComponent_Basic()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPowerComponent_Basic::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPowerComponent_Basic::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UPowerComponent_Basic::GetPower() const
{
    return PowerDraw;
}

