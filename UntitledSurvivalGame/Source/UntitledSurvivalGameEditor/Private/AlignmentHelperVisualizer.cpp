// Fill out your copyright notice in the Description page of Project Settings.


#include "AlignmentHelperVisualizer.h"
#include "Dev/MapGeneration/AlignmentHelperComponent.h"
#include "Dev/MapGeneration/MapGeneratorTypes.h"

void FAlignmentHelperVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	FVector Origin = Component->GetOwner()->GetActorLocation();
	FRotator Rotation = Component->GetOwner()->GetActorRotation();
	//DrawWireSphere(PDI, Origin, FColor::Red, 16, 8, 0, 2);
	//DrawWireBox(PDI, FBox(Origin + FVector(-250, -250, -5), Origin + FVector(250, 250, 5)), FColor::White, 0, 2);

	const UAlignmentHelperComponent* Comp = Cast<UAlignmentHelperComponent>(Component);
	if(!Comp) return;
	FVector Location;
	//FVector Normal;
	FRotator Rotator;
	uint8 Size;
	TArray<FVector> DoorPoints;
	int32 MaxNum = Comp->GetNumDoors();
	for (int32 i = 0; i < MaxNum; i++) {
		if (!Comp->GetDoor(i, Location, Rotator, Size)) continue;
		//Normal = Rotation.RotateVector(Normal);
		Location = Rotation.RotateVector(Location);
		Rotator = (Rotation.Quaternion() * Rotator.Quaternion()).Rotator();
		//DrawDirectionalArrow(PDI, FTransform(Normal.Rotation(), Origin + Location, FVector::OneVector).ToMatrixWithScale(), FColor::White, 50, 4, 0, 2);
		DrawDirectionalArrow(PDI, FTransform(Rotator.RotateVector(FVector::ForwardVector).Rotation(), Origin + Location + (FVector::UpVector * 50), FVector::OneVector).ToMatrixWithScale(), FColor::White, 50, 4, 0, 2);
		GetDoorPoints(Size, DoorPoints);
		DrawDashedLine(PDI, Origin + Location + Rotator.RotateVector(DoorPoints[0]), Origin + Location + Rotator.RotateVector(DoorPoints[1]), FColor::White, 8, 0);
		DrawDashedLine(PDI, Origin + Location + Rotator.RotateVector(DoorPoints[1]), Origin + Location + Rotator.RotateVector(DoorPoints[2]), FColor::White, 8, 0);
		DrawDashedLine(PDI, Origin + Location + Rotator.RotateVector(DoorPoints[2]), Origin + Location + Rotator.RotateVector(DoorPoints[3]), FColor::White, 8, 0);
		DrawDashedLine(PDI, Origin + Location + Rotator.RotateVector(DoorPoints[3]), Origin + Location + Rotator.RotateVector(DoorPoints[0]), FColor::White, 8, 0);
	}
	
	//*
	FTileShape ShapeRef = Comp->GetShape();
	ShapeRef.BuildTiles();
	TArray<FTile> ContainedTiles = TArray<FTile>(ShapeRef.ContainedTiles);
	TArray<FVector> TilePoints;
	GetTilePoints(Comp->GetTileSize(), TilePoints);
	FVector TileLocation;
	int32 TileSize = Comp->GetTileSize();
	DrawDashedLine(PDI, Origin, Origin + FVector::UpVector * 10, FColor::White, 1, 0);
	for (int32 i = 0; i < ContainedTiles.Num(); i++) {
		DrawDashedLine(PDI, Origin + FVector::RightVector * 5 * i, Origin + FVector::UpVector * 10 * i, FColor::White, 1, 0);
		TileLocation = Origin + Rotation.RotateVector(ContainedTiles[i].Location.AsVector() * Comp->GetTileSize());
		DrawWireSphere(PDI, TileLocation, FColor::Cyan, 8, 4, 0, 1);
		if (ContainedTiles[i].IsExternalEdge[0]) DrawDashedLine(PDI, TileLocation + Rotation.RotateVector(TilePoints[0]), TileLocation + Rotation.RotateVector(TilePoints[1]), FColor::Cyan, 1, 0);
		if (ContainedTiles[i].IsExternalEdge[1]) DrawDashedLine(PDI, TileLocation + Rotation.RotateVector(TilePoints[1]), TileLocation + Rotation.RotateVector(TilePoints[2]), FColor::Cyan, 1, 0);
		if (ContainedTiles[i].IsExternalEdge[2]) DrawDashedLine(PDI, TileLocation + Rotation.RotateVector(TilePoints[2]), TileLocation + Rotation.RotateVector(TilePoints[3]), FColor::Cyan, 1, 0);
		if (ContainedTiles[i].IsExternalEdge[3]) DrawDashedLine(PDI, TileLocation + Rotation.RotateVector(TilePoints[3]), TileLocation + Rotation.RotateVector(TilePoints[0]), FColor::Cyan, 1, 0);
	}
	//*/
}

void FAlignmentHelperVisualizer::GetDoorPoints(uint8 DoorSize, TArray<FVector>& Points) const
{
	Points.Empty();
	switch (DoorSize) {
		case 1:
			Points.Add(FVector(0, 50, 0));
			Points.Add(FVector(0, -50, 0));
			Points.Add(FVector(0, -50, 205));
			Points.Add(FVector(0, 50, 205));
			break;
		default:
			Points.Add(FVector(0, 190, 0));
			Points.Add(FVector(0, -190, 0));
			Points.Add(FVector(0, -190, 300));
			Points.Add(FVector(0, 190, 300));
			break;
	}
}

void FAlignmentHelperVisualizer::GetTilePoints(int32 TileSize, TArray<FVector>& Points) const
{
	Points.Empty();
	Points.Add(FVector((FVector::ForwardVector)      + (FVector::RightVector * -1)) * TileSize * 0.5f);
	Points.Add(FVector((FVector::ForwardVector)      + (FVector::RightVector))      * TileSize * 0.5f);
	Points.Add(FVector((FVector::ForwardVector * -1) + (FVector::RightVector))      * TileSize * 0.5f);
	Points.Add(FVector((FVector::ForwardVector * -1) + (FVector::RightVector * -1)) * TileSize * 0.5f);
}
