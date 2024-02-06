// Fill out your copyright notice in the Description page of Project Settings.


#include "Dev/MapGeneration/AlignmentHelper.h"
#if WITH_EDITOR
#include "DrawDebugHelpers.h"
#include "Components/ArrowComponent.h"
#endif

// Sets default values
AAlignmentHelper::AAlignmentHelper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (Mesh) {
		Mesh->SetupAttachment(RootComponent);
	}
	
#if WITH_EDITOR
	TileSize = 400;
#endif
}

FTileShape AAlignmentHelper::GetTileShape() const
{
	return TileShape;
}

// Called when the game starts or when spawned
void AAlignmentHelper::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAlignmentHelper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#if WITH_EDITOR
void AAlignmentHelper::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	//FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	//if (PropertyName == GET_MEMBER_NAME_CHECKED(AAlignmentHelper, TileShape) ) {
	//	RebuildShape();
	//}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void AAlignmentHelper::RebuildShape()
{
	TileShape.BuildTiles();
}

