// Fill out your copyright notice in the Description page of Project Settings.


#include "Dev/MapGeneration/AlignmentHelperComponent.h"
#include "Dev/MapGeneration/AlignmentHelper.h"
#include "Dev/MapGeneration/MapGeneratorTypes.h"

// Sets default values for this component's properties
UAlignmentHelperComponent::UAlignmentHelperComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAlignmentHelperComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAlignmentHelperComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int32 UAlignmentHelperComponent::GetNumDoors() const
{
    return DoorTiles.Num();
}

bool UAlignmentHelperComponent::GetDoor(int32 Index, FVector& Location, FRotator& Rotation, uint8& Size) const
{
	if(!DoorTiles.IsValidIndex(Index)) return false;
	Location	= DoorTiles[Index].Location;
	//Normal		= DoorTiles[Index].Normal;
	Size		= DoorTiles[Index].DoorSize;
	Rotation	= DoorTiles[Index].Rotation;
	return true;
}

FTileShape UAlignmentHelperComponent::GetShape() const
{
	AAlignmentHelper* Helper = Cast<AAlignmentHelper>(GetOwner());
	if(!IsValid(Helper)) return FTileShape();
	return Helper->GetTileShape();
}

TArray<FVector> UAlignmentHelperComponent::GetContainedTiles() const
{
	AAlignmentHelper* Helper = Cast<AAlignmentHelper>(GetOwner());
	if (!IsValid(Helper)) return TArray<FVector>();
	return Helper->GetTileShape().GetContainedTileLocations();
}

int32 UAlignmentHelperComponent::GetTileSize() const
{
	AAlignmentHelper* Helper = Cast<AAlignmentHelper>(GetOwner());
	if (!IsValid(Helper)) return 1;
	return Helper->TileSize;
}

