// Fill out your copyright notice in the Description page of Project Settings.


#include "Dev/MapGeneration/DevMapGenerator_v2.h"

// Sets default values
ADevMapGenerator_v2::ADevMapGenerator_v2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoundaryLocations.Add(FVector::ZeroVector);
	BoundaryDoors.Add(FDoorInfo(FVector(0,0,0), EGridDirection::N, EConnectionType::MEDIUM));
	TileSize = 250;
	WallThickness = 8;

	DebugShapeIndex = -1;
	DebugDoorAtttachmentIndex = -1;
	
	Tiles.Empty();

	/*
	FTileShape Shape;
	TArray<FVector> Locations;
	Locations.Add(FVector(0,0,0));
	TArray<FDoorInfo> Doors;
	Doors.Add(FDoorInfo(FVector(0, 0, 0), EGridDirection::N, EConnectionType::MEDIUM));
	Doors.Add(FDoorInfo(FVector(0, 0, 0), EGridDirection::E, EConnectionType::MEDIUM));
	Doors.Add(FDoorInfo(FVector(0, 0, 0), EGridDirection::S, EConnectionType::MEDIUM));
	Doors.Add(FDoorInfo(FVector(0, 0, 0), EGridDirection::W, EConnectionType::MEDIUM));
	Shape.ContainedTileLocations = Locations;
	Shape.DoorInfo = Doors;
	Shape.BuildEdges();
	TileShapes.Add(Shape);

	Locations.Empty();
	Doors.Empty();
	Locations.Add(FVector(0, 0, 0));
	Locations.Add(FVector(1, 0, 0));
	Locations.Add(FVector(0, 1, 0));
	Locations.Add(FVector(0, 2, 0));
	Doors.Add(FDoorInfo(FVector(1, 0, 0), EGridDirection::N, EConnectionType::MEDIUM));
	Doors.Add(FDoorInfo(FVector(0, 2, 0), EGridDirection::E, EConnectionType::MEDIUM));
	Doors.Add(FDoorInfo(FVector(0, 0, 0), EGridDirection::S, EConnectionType::MEDIUM));
	Doors.Add(FDoorInfo(FVector(0, 0, 0), EGridDirection::W, EConnectionType::MEDIUM));
	Shape.ContainedTileLocations = Locations;
	Shape.DoorInfo = Doors;
	Shape.BuildEdges();
	TileShapes.Add(Shape);

	Locations.Empty();
	Doors.Empty();
	Locations.Add(FVector(0, 0, 0));
	Locations.Add(FVector(1, 0, 0));
	Locations.Add(FVector(0, 1, 0));
	Locations.Add(FVector(1, 1, 0));
	Shape.ContainedTileLocations = Locations;
	Shape.DoorInfo = Doors;
	Shape.BuildEdges();
	TileShapes.Add(Shape);

	Locations.Empty();
	Doors.Empty();
	Locations.Add(FVector(0, 0, 0));
	Locations.Add(FVector(1, 0, 0));
	Locations.Add(FVector(1, 1, 0));
	Locations.Add(FVector(2, 1, 0));
	Shape.ContainedTileLocations = Locations;
	Shape.DoorInfo = Doors;
	Shape.ShapeRotation = FRotator::ZeroRotator;
	Shape.BuildEdges();
	TileShapes.Add(Shape);

	Locations.Empty();
	Doors.Empty();
	Locations.Add(FVector(0, 0, 0));
	Locations.Add(FVector(0, 1, 0));
	Locations.Add(FVector(-1, 1, 0));
	Locations.Add(FVector(-1, 2, 0));
	Shape.ContainedTileLocations = Locations;
	Shape.DoorInfo = Doors;
	Shape.ShapeRotation = FRotator::ZeroRotator;
	Shape.BuildEdges();
	TileShapes.Add(Shape);

	Locations.Empty();
	Doors.Empty();
	Locations.Add(FVector(0, 0, 0));
	Locations.Add(FVector(0, 1, 0));
	Locations.Add(FVector(1, 1, 0));
	Locations.Add(FVector(1, 2, 0));
	Shape.ContainedTileLocations = Locations;
	Shape.DoorInfo = Doors;
	Shape.ShapeRotation = FRotator::ZeroRotator;
	Shape.BuildEdges();
	TileShapes.Add(Shape);

	Locations.Empty();
	Doors.Empty();
	Locations.Add(FVector(0, 0, 0));
	Locations.Add(FVector(0, 1, 0));
	Locations.Add(FVector(0, 2, 0));
	Locations.Add(FVector(0, 3, 0));
	Shape.ContainedTileLocations = Locations;
	Shape.DoorInfo = Doors;
	Shape.ShapeRotation = FRotator::ZeroRotator;
	Shape.BuildEdges();
	TileShapes.Add(Shape);
	//*/
}

// Called when the game starts or when spawned
void ADevMapGenerator_v2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADevMapGenerator_v2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//*
	for (int i = 0; i < Tiles.Num(); i++) {
		//if(!IsValid(Tiles[i])) continue;
		DrawTile(Tiles[i]);
	}

	for (const FDoorInfo Info : BoundaryDoors) {
		//Info.Draw(GetWorld(), TileSize * 2, GetActorLocation(), GetActorRotation(), FColor::Red, 40);
	}
	
	for (const FDoorInfo Info : FinishedDoors) {
		//Info.Draw(GetWorld(), TileSize * 2, GetActorLocation(), GetActorRotation(), FColor::Green, 40);
	}
	//*/
}

bool ADevMapGenerator_v2::GenerateTileIterator_Implementation()
{
	if(BoundaryDoors.Num() == 0) return false;
	int32 index = FMath::RandRange(0, BoundaryDoors.Num() - 1);
	if(!BoundaryDoors.IsValidIndex(index)) return false;
	FVector Loc = BoundaryDoors[index].Location.AsVector();
	FRotator Rot = FTileCoordinate::GetDirectionAsRotator(BoundaryDoors[index].Direction);
	FVector ModifiedLocation = Loc;
	FRotator ModifiedRotation = Rot;
	int32 TileShapeIndex = FMath::RandRange(0, TileShapes.Num()-1);
	if (TileShapes.IsValidIndex(DebugShapeIndex)) TileShapeIndex = DebugShapeIndex;
	if (!TileShapes.IsValidIndex(TileShapeIndex)) {
		GEngine->AddOnScreenDebugMessage(0, 10, FColor::Red, "Got bad TileShapes index");
	}
	if (CanMakeTileShapeAt(ModifiedLocation, ModifiedRotation, BoundaryDoors[index], TileShapeIndex)) {
		MakeTileShape(index, TileShapeIndex, ModifiedLocation, ModifiedRotation);
	}
	return true;
}

bool ADevMapGenerator_v2::CleanTiles_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White, FString::Printf(TEXT("NumTiles: %i"), int32(Tiles.Num())));
	if(Tiles.Num() == 0) return false;
	for (int32 i = 0; i < Tiles.Num(); i++) {
		for(int32 k = 0; k < Tiles[i]->Doors.Num(); k++){
			Tiles[i]->AddConnectionInternal(Tiles[i]->Doors[k], Tiles[i], EConnectionType::MEDIUM, true);
		}
	}
	for (int32 i = 0; i < Tiles.Num(); i++) {
		//if(!Tiles.IsValidIndex(i)) continue;
		//if(!IsValid(Tiles[i])) continue;
		Tiles[i]->CleanConnections(this);
	}
	return true;
}

bool ADevMapGenerator_v2::FixPathing_Implementation()
{
	// Make sure all tiles are reachable from all tiles.
	TArray<UMapTile*> OpenList;
	TArray<UMapTile*> CloseList;
	TArray<UMapTile*> AllNeighbors;
	//TArray<UMapTile*> ClosedList;
	int32 Counter = 0;
	OpenList.Add(Tiles[0]);
	while (CloseList.Num() != Tiles.Num() && Counter++ < 500) {
		GetConnectedPaths(OpenList, CloseList);
		if (CloseList.Num() != Tiles.Num()) {
			bool FoundOne = false;
			for (UMapTile* tile : CloseList) {
				if (FoundOne) break;
				for (FConnection Neighbor : tile->Neighbors) {
					if(FoundOne) break;
					if (!OpenList.Contains(Neighbor.TileRef) && !CloseList.Contains(Neighbor.TileRef)) {
						tile->AddConnection(Neighbor.Direction, Neighbor.TileRef);
						if (Neighbor.TileRef->GetConnections().Num() == 0) {
							CloseList.Add(Neighbor.TileRef);
						}
						else {
							OpenList.Add(Neighbor.TileRef);
							FoundOne = true;
						}
					}
				}
			}
		}
	}
	//ClosedList = CloseList;
	return true;
}

void ADevMapGenerator_v2::PostBuildCleanup_Implementation()
{
	Tiles.Empty();

	BoundaryLocations.Empty();
	
	BoundaryDoors.Empty();

	FinishedLocations.Empty();

	FinishedDoors.Empty();

	TileShapes.Empty();
}

void ADevMapGenerator_v2::GetConnectedPaths(TArray<UMapTile*>& OpenList, TArray<UMapTile*>& CloseList)
{
	UMapTile* TileRef;
	TArray<FConnection> Connections;
	int32 Counter = 0;
	while (OpenList.Num() > 0 && ++Counter < Tiles.Num()) {
		TileRef = OpenList[0];
		OpenList.RemoveAt(0);
		CloseList.Add(TileRef);
		Connections = TileRef->GetConnections();
		for (FConnection C : Connections) {
			//if(C.TileRef == TileRef) GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Black, FString::Printf(TEXT("WTF?")));
			if (!OpenList.Contains(C.TileRef) && !CloseList.Contains(C.TileRef)) {
				OpenList.Add(C.TileRef);
			}
		}
		TileRef = nullptr;
		//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Black, FString::Printf(TEXT("%i: OpenList: %i, ClosedList: %i"), Counter, OpenList.Num(), ClosedList.Num()));
		Connections.Empty();
	}
}

bool ADevMapGenerator_v2::CanMakeTileAt(FVector Location, FVector Offset, FRotator Rotation) const
{
	return !TileExistsAtLocation(Location);
}

UMapTile* ADevMapGenerator_v2::MakeTile(FVector Location, FRotator Rotation)
{
	FVector FlattenedLocation = FVector(FMath::RoundToInt(Location.X), FMath::RoundToInt(Location.Y), FMath::RoundToInt(Location.Z));
	if(!CanMakeTileAt(FlattenedLocation)) return nullptr;
	UMapTile* NewTile = NewObject<UMapTile>(this, UMapTile::StaticClass());
	NewTile->Location = FlattenedLocation;
	Tiles.Add(NewTile);
	return NewTile;
}

bool ADevMapGenerator_v2::CanMakeTileShapeAt(FVector& Location, FRotator& Rotation, FDoorInfo& AlignmentDoor, int32& TileShapeIndex) const
{
	bool CanPlaceShape = true;
	if(!TileShapes.IsValidIndex(TileShapeIndex)) return false;
	FTileShape Shape = TileShapes[TileShapeIndex];
	FTransform AlignmentTransform;
	AlignmentTransform.SetLocation(Location);
	AlignmentTransform.SetRotation(Rotation.Quaternion());
	FDoorInfo Door = AlignmentDoor;//.GetInverse();
	for (int32 i = 0; i < Shape.DoorInfo.Num(); i++) {
		//int32 i = 0;
		//if(Shape.DoorInfo.IsValidIndex(DebugDoorAtttachmentIndex)) i = DebugDoorAtttachmentIndex;
		CanPlaceShape = true;
		Shape.AlignToEdge(Shape.DoorInfo[i], Door, AlignmentTransform);
		for (int32 k = 0; k < TileShapes[TileShapeIndex].ContainedTileLocations.Num(); k++) {
			if (TileExistsAtLocation(AlignmentTransform.GetLocation() + AlignmentTransform.GetRotation().RotateVector(TileShapes[TileShapeIndex].ContainedTileLocations[k]))) {
				CanPlaceShape = false;
				break;
			}
		}
		if(CanPlaceShape) break;
	}
	if (CanPlaceShape) {
		Location = AlignmentTransform.GetLocation();
		Rotation = AlignmentTransform.GetRotation().Rotator();// + FRotator(0,0,0);
	}
	return CanPlaceShape;
}

bool ADevMapGenerator_v2::MakeTileShape(int32 BoundaryIndex, int32 TileShapeIndex, FVector Location, FRotator Rotation)
{
	FVector FlattenedLocation = FVector(FMath::RoundToInt(Location.X), FMath::RoundToInt(Location.Y), FMath::RoundToInt(Location.Z));
	FVector Loc = FlattenedLocation;
	FRotator Rot = Rotation;
	if (!CanMakeTileShapeAt(Loc, Rot, BoundaryDoors[BoundaryIndex], TileShapeIndex)) return false;
	if (TileShapes[TileShapeIndex].ContainedTiles.Num() == 0) TileShapes[TileShapeIndex].BuildTiles();
	
	FinishedDoors.Add(BoundaryDoors[BoundaryIndex]);
	BoundaryDoors.RemoveAt(BoundaryIndex);

	TArray<EGridDirection> Neighbors;
	Neighbors.Add(EGridDirection::N);
	Neighbors.Add(EGridDirection::E);
	Neighbors.Add(EGridDirection::S);
	Neighbors.Add(EGridDirection::W);

	TArray<FVector> NeighborTiles;
	NeighborTiles.Add(FVector::ForwardVector);
	NeighborTiles.Add(FVector::RightVector);
	NeighborTiles.Add(FVector::ForwardVector * -1);
	NeighborTiles.Add(FVector::RightVector * -1);

	FTileShape ShapeCopy = FTileShape(TileShapes[TileShapeIndex]);
	
	for (int32 i = 0; i < TileShapes[TileShapeIndex].ContainedTiles.Num(); i++) {
		UMapTile* Tile = MakeTile(Loc + Rot.RotateVector(TileShapes[TileShapeIndex].ContainedTiles[i].Location.AsVector()));

		Tile->OriginalCoordinate = FVector2D(TileShapes[TileShapeIndex].ContainedTiles[i].Location.X, TileShapes[TileShapeIndex].ContainedTiles[i].Location.Y);

		if (i == 0) {
			ShapeCopy.IsValidDrawTarget = true;
			Tile->LinkedShape = ShapeCopy;
			Tile->LinkedShape.ShapeRotation = Rotation;
		}

		Tile->OriginalDoor = -1;
		if (ShapeCopy.DoorInfo.IsValidIndex(DebugDoorAtttachmentIndex)) {
			if (TileShapes[TileShapeIndex].ContainedTiles[i].Location.Equals(ShapeCopy.DoorInfo[DebugDoorAtttachmentIndex].Location)) {
				Tile->OriginalDoor = -3;
			}
			else {
				Tile->OriginalDoor = -2;
			}
		}

		for (int32 k = 0; k < TileShapes[TileShapeIndex].ContainedTiles[i].ContainedDoors.Num(); k++) {
			FDoorInfo Info	= FDoorInfo();
			Info.Location	= FTileCoordinate(Tile->Location);
			Info.Direction	= FTileCoordinate::RotateDirectionByRotator(TileShapes[TileShapeIndex].ContainedTiles[i].ContainedDoors[k].Direction, Rot);
			Info.Type		= TileShapes[TileShapeIndex].ContainedTiles[i].ContainedDoors[k].Type;
			Tile->Doors.AddUnique(Info.Direction);
			if (Tile->OriginalDoor == -3) {
				if (ShapeCopy.DoorInfo[DebugDoorAtttachmentIndex] == TileShapes[TileShapeIndex].ContainedTiles[i].ContainedDoors[k]) {
					Tile->OriginalDoor = int32(k);
				}
			}
			FinishedDoors.AddUnique(Info);
			if (FinishedDoors.Contains(Info.GetInverse())) continue;
			if (TileExistsAtLocation(Info.GetInverse().Location.AsVector())) continue;
			BoundaryDoors.AddUnique(Info.GetInverse());
		}
	}
	return true;
}

bool ADevMapGenerator_v2::TileExistsAtLocation(FVector Location) const
{
	return GetTileAtLocation(Location) != INDEX_NONE;
}

int32 ADevMapGenerator_v2::GetTileAtLocation(FVector Location) const
{
	int32 index = INDEX_NONE;
	for (int32 i = 0; i < Tiles.Num(); i++) {
		if (Tiles[i]->GetRelativeLocation().Equals(Location)) {
			index = i;
			break;
		}
	}
	return index;
}

TArray<FTransform> ADevMapGenerator_v2::GetAllConnectionLocations(TArray<bool>& IsOpen) const
{
	TArray<FTileCoordinate> Doors;
	TArray<FRotator> Rot;
	for (int32 i = 0; i < Tiles.Num(); i++) {
		for (int32 k = 0; k < Tiles[i]->Connect.Num(); k++) {
			FTileCoordinate T = FTileCoordinate((Tiles[i]->Location * TileSize * 2) + FTileCoordinate::GetDirectionAsVector(Tiles[i]->Connect[k].Direction) * TileSize);
			if (!Doors.Contains(T)) {
				Rot.Add(FTileCoordinate::GetDirectionAsRotator(FTileCoordinate::GetInverseDirection(Tiles[i]->Connect[k].Direction)));
				Doors.Add(T);
				IsOpen.Add(Tiles[i]->Connect[k].IsOpen);
			}
		}
	}
	TArray<FTransform> Locations;
	FTransform T;
	T.SetScale3D(FVector::OneVector);
	for (int32 i = 0; i < Doors.Num(); i++) {
		T.SetLocation(Doors[i].AsVector());
		T.SetRotation(Rot[i].Quaternion());
		Locations.Add(T);
	}
	return Locations;
}

TArray<FTransform> ADevMapGenerator_v2::GetAllShapes(TArray<FTileShape>& Shapes) const
{
	TArray<FTransform> Locations;
	for (int32 i = 0; i < Tiles.Num(); i++) {
		if (Tiles[i]->LinkedShape.IsValidDrawTarget) {
			Locations.Add(FTransform(Tiles[i]->LinkedShape.ShapeRotation, Tiles[i]->Location * TileSize * 2, FVector::OneVector));
			Shapes.Add(Tiles[i]->LinkedShape);
		}
	}
	return Locations;
}

void ADevMapGenerator_v2::AddBoundaryLocation(FVector Location)
{
	if(FinishedLocations.Contains(Location)) return;
	BoundaryLocations.AddUnique(Location);
}

void ADevMapGenerator_v2::DrawTile(UMapTile* Tile, FColor Color)
{
	FVector Loc = GetActorLocation() + GetActorRotation().RotateVector(Tile->GetWorldLocation(TileSize));
	FQuat Rot = GetActorRotation().Quaternion();
	bool DrawDoors = true;
	int32 AdjustedTileSize = TileSize - (WallThickness * 0.5);
	//DrawDebugLine(GetWorld(), Loc + GetActorRotation().RotateVector(FVector(0 - AdjustedTileSize, 0 - AdjustedTileSize, 0)), Loc + GetActorRotation().RotateVector(FVector(AdjustedTileSize, 0 - AdjustedTileSize, 0)), FColor::Cyan);
	//DrawDebugLine(GetWorld(), Loc + GetActorRotation().RotateVector(FVector(AdjustedTileSize, 0 - AdjustedTileSize, 0)), Loc + GetActorRotation().RotateVector(FVector(AdjustedTileSize, AdjustedTileSize, 0)), FColor::Cyan);
	//DrawDebugLine(GetWorld(), Loc + GetActorRotation().RotateVector(FVector(AdjustedTileSize, AdjustedTileSize, 0)), Loc + GetActorRotation().RotateVector(FVector(0 - AdjustedTileSize, AdjustedTileSize, 0)), FColor::Cyan);
	//DrawDebugLine(GetWorld(), Loc + GetActorRotation().RotateVector(FVector(0 - AdjustedTileSize, AdjustedTileSize, 0)), Loc + GetActorRotation().RotateVector(FVector(0 - AdjustedTileSize, 0 - AdjustedTileSize, 0)), FColor::Cyan);

	int32 Rotation = -1;
	if (Tile->LinkedShape.IsValidDrawTarget) {
		DrawNormal(Loc + GetActorRotation().RotateVector(FVector::UpVector * 10), Rot.Rotator());
		FTransform ShapeTransform;
		ShapeTransform.SetLocation(Loc);
		ShapeTransform.SetRotation(Rot);
		Tile->LinkedShape.DrawShape(GetWorld(), ShapeTransform, TileSize * 2);
		Rotation = Tile->LinkedShape.ShapeRotation.Yaw;
	}

	FString str = FString::Printf(TEXT("Rotation: %i\nActual x: %i, y: %i\nLoaded x: %i, y: %i\nOriginal: %i"), Rotation, (int)Tile->Location.X, (int)Tile->Location.Y, (int)Tile->OriginalCoordinate.X, (int)Tile->OriginalCoordinate.Y, (int)Tile->OriginalDoor);
	//DrawDebugString(GetWorld(), Loc, str);

	if (DrawDoors) {
		for (int32 i = 0; i < Tile->Connect.Num(); i++) {
			DrawConnection(Tile->Connect[i], Tile, FColor::White); //FConnection(Tile->Doors[i], Tile, EConnectionType::MEDIUM)
		}
	}
}

void ADevMapGenerator_v2::DrawNormal(FVector Location, FRotator Rotation, FColor First, FColor Second, FColor Third)
{
	DrawDebugDirectionalArrow(GetWorld(), Location, Location + (Rotation.RotateVector(FVector::ForwardVector) * 50), 20, First);
	DrawDebugLine(GetWorld(), Location, Location + (Rotation.RotateVector(FVector::RightVector) * 30), Second, false, 0, 0, 2);
	DrawDebugLine(GetWorld(), Location, Location + (Rotation.RotateVector(FVector::UpVector) * 30), Third, false, 0, 0, 2);
}

void ADevMapGenerator_v2::DrawConnection(FConnection Connection, UMapTile* ParentTile, FColor Color)
{
	if(!IsValid(ParentTile)) return;
	FVector Loc = GetActorLocation() + GetActorRotation().RotateVector(ParentTile->GetWorldLocation(TileSize) + FTileCoordinate::GetDirectionAsVector(Connection.Direction, false) * TileSize);
	FQuat Rot = GetActorRotation().Quaternion() * GetActorRotation().RotateVector(FTileCoordinate::GetDirectionAsVector(Connection.Direction, false)).Rotation().Quaternion();
	TArray<FVector> Points;
	switch (Connection.ConnectionType) {
		case EConnectionType::FULL: 
			Points.Add(FVector(0, -TileSize, 0));
			Points.Add(FVector(-WallThickness, -TileSize, 0));
			Points.Add(FVector(0, TileSize, 0));
			Points.Add(FVector(-WallThickness, TileSize, 0));
			break;
		case EConnectionType::LARGE:
			Points.Add(FVector(0, -150, 0));
			Points.Add(FVector(-WallThickness, -150, 0));
			Points.Add(FVector(0, 150, 0));
			Points.Add(FVector(-WallThickness, 150, 0));
			break;
		case EConnectionType::MEDIUM:
			Points.Add(FVector(0, -100, 0));
			Points.Add(FVector(-WallThickness, -100, 0));
			Points.Add(FVector(0, 100, 0));
			Points.Add(FVector(-WallThickness, 100, 0));
			break;
		case EConnectionType::SMALL:
			Points.Add(FVector(0, -50, 0));
			Points.Add(FVector(-WallThickness, -50, 0));
			Points.Add(FVector(0, 50, 0));
			Points.Add(FVector(-WallThickness, 50, 0));
			break;
		case EConnectionType::TINY:
			Points.Add(FVector(0, -20, 0));
			Points.Add(FVector(-WallThickness, -20, 0));
			Points.Add(FVector(0, 20, 0));
			Points.Add(FVector(-WallThickness, 20, 0));
			break;
		default:
			break;
	}
	//DrawDebugLine(GetWorld(), Loc, GetActorLocation() + ParentTile->GetWorldLocation(TileSize), FColor::Red);
	//if(IsValid(Connection.TileRef)) DrawDebugLine(GetWorld(), Loc, GetActorLocation() + Connection.TileRef->GetWorldLocation(TileSize) + FVector::UpVector * 10, FColor::Green);
	FColor ModColor = (Connection.IsOpen ? Color : FColor::Red);
	DrawDebugLine(GetWorld(), Loc + Rot.Rotator().RotateVector(Points[0]), Loc + Rot.Rotator().RotateVector(Points[1]), ModColor, false, -1, 0, 2);
	DrawDebugLine(GetWorld(), Loc + Rot.Rotator().RotateVector(Points[2]), Loc + Rot.Rotator().RotateVector(Points[3]), ModColor, false, -1, 0, 2);
	DrawDebugLine(GetWorld(), Loc + Rot.Rotator().RotateVector(Points[1]), Loc + Rot.Rotator().RotateVector(Points[3]), ModColor, false, -1, 0, 2);
	//FText EnumName;
	//UEnum::GetDisplayValueAsText(Connection.Direction, EnumName);
	//DrawDebugString(GetWorld(), Loc, EnumName.ToString());
	/*
	TArray<FVector> Points;
	FRotator DoorRotation = Door.Rotation;
	FVector RotatedNormal = (ParentRotation.Quaternion() * Door.Rotation.Quaternion()).Vector();
	FQuat RotatedDirection = ParentRotation.Quaternion() * Door.Rotation.Quaternion();
	switch (Door.DoorSize) {
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
	FVector Loc = ParentLocation + ParentRotation.RotateVector(Door.Location);
	DrawDebugLine(GetWorld(), Loc + RotatedDirection.RotateVector(Points[0]), Loc + RotatedDirection.RotateVector(Points[1]), Color);
	DrawDebugLine(GetWorld(), Loc + RotatedDirection.RotateVector(Points[1]), Loc + RotatedDirection.RotateVector(Points[2]), Color);
	DrawDebugLine(GetWorld(), Loc + RotatedDirection.RotateVector(Points[2]), Loc + RotatedDirection.RotateVector(Points[3]), Color);
	DrawDebugLine(GetWorld(), Loc + RotatedDirection.RotateVector(Points[3]), Loc + RotatedDirection.RotateVector(Points[0]), Color);
	//DrawDebugLine(GetWorld(), Loc, ParentLocation, Color);
	//DrawDebugDirectionalArrow(GetWorld(), Loc + ParentRotation.RotateVector(FVector::UpVector * 50), Loc + ParentRotation.RotateVector(FVector::UpVector * 50) + RotatedNormal * 50, 20, Color, false, -1.f, 0, 2.f);
	DrawNormal(Loc + ParentRotation.RotateVector(FVector::UpVector * 50), RotatedDirection.Rotator());
	//*/
}








UMapTile::UMapTile()
{
	HadConnectionRemoved = false;
	OriginalDoor = -1;
}

FVector UMapTile::GetWorldLocation(int32 TileSize) const
{
	return Location * (TileSize * 2);
}

FVector UMapTile::GetRelativeLocation() const
{
	return Location;
}

TArray<FConnection> UMapTile::GetConnections() const
{
	return Connect;
}

void UMapTile::SetOriginalCoordinate(FVector Vector)
{
	OriginalCoordinate = FVector2D(Vector.X, Vector.Y);
}

bool UMapTile::CleanConnections_Implementation(ADevMapGenerator_v2* GeneratorRef)
{
	int32 index = INDEX_NONE;
	int32 index2 = INDEX_NONE;
	for (int32 i = 0; i < Connect.Num(); i++){//int i = Connect.Num() - 1; i >= 0; i--) {
		index = GeneratorRef->GetTileAtLocation(GetRelativeLocation() + FTileCoordinate::GetDirectionAsVector(Connect[i].Direction, false));
		//GEngine->AddOnScreenDebugMessage(-1,10,FColor::Cyan, FString::Printf(TEXT("[%i]IsValid: %s"), index, (GeneratorRef->Tiles.IsValidIndex(index) ? "true" : "false")));
		if (GeneratorRef->Tiles.IsValidIndex(index)) {
			index2 = GeneratorRef->Tiles[index]->FindConnection(FTileCoordinate::GetInverseDirection(Connect[i].Direction), this);
			//GEngine->AddOnScreenDebugMessage(-1,10,FColor::White,FString::Printf(TEXT("[%i/%i].IsOpen: %i"), index, Connect.Num(), index2));
			if (GeneratorRef->Tiles[index]->Connect.IsValidIndex(index2)) {
				Connect[i].TileRef = GeneratorRef->Tiles[index];
				GeneratorRef->Tiles[index]->Connect[index2].TileRef = this;
				Connect[i].IsOpen = true;
				GeneratorRef->Tiles[index]->Connect[index2].IsOpen = true;
				//GEngine->AddOnScreenDebugMessage(-1,10,FColor::Red,"IsOpen");
			}
			else {
				Connect[i].IsOpen = false;
				HadConnectionRemoved = true;
			}
		}
		else {
			//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, "Removing connection!");
			//Connect.RemoveAt(i);
			Connect[i].IsOpen = false;
			HadConnectionRemoved = true;
		}
	}
	index = INDEX_NONE;
	for (int i = 0; i < Connect.Num(); i++) {
		//if(!Connect[i].IsOpen) continue;
		index = GeneratorRef->GetTileAtLocation(GetRelativeLocation() + FTileCoordinate::GetDirectionAsVector(Connect[i].Direction, false));
		if (!GeneratorRef->Tiles.IsValidIndex(index)) continue;
		index2 = GeneratorRef->Tiles[index]->FindConnection(FTileCoordinate::GetInverseDirection(Connect[i].Direction), this);
		if (!GeneratorRef->Tiles[index]->Connect.IsValidIndex(index2)) continue;
		FVector Dir = GetRelativeLocation() + FTileCoordinate::GetDirectionAsVector(Connect[i].Direction, false);

		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("Adding... Equal? %s"), (this == GeneratorRef->Tiles[index] ? TEXT("True") : TEXT("false"))));
		//GeneratorRef->Tiles[index]->AddConnection(FTileCoordinate::GetInverseDirection(Connect[i].Direction), this, Connect[i].ConnectionType, true);
		//GeneratorRef->Tiles[index]->AddConnection(Connect[i].Direction, GeneratorRef->Tiles[index], Connect[i].ConnectionType, true);

		Connect[i].IsOpen = true;
		GeneratorRef->Tiles[index]->Connect[index2].IsOpen = true;
	}
	index = INDEX_NONE;
	TArray<EGridDirection> NeighborTiles;
	NeighborTiles.Add(EGridDirection::N);
	NeighborTiles.Add(EGridDirection::S);
	NeighborTiles.Add(EGridDirection::E);
	NeighborTiles.Add(EGridDirection::W);
	for (EGridDirection Dir : NeighborTiles) {
		index = GeneratorRef->GetTileAtLocation(Location + FTileCoordinate::GetDirectionAsVector(Dir, false));
		if (GeneratorRef->Tiles.IsValidIndex(index)) {
			Neighbors.Add(FConnection(Dir, GeneratorRef->Tiles[index], EConnectionType::INVALID));
		}
	}
	return true;
}

bool UMapTile::AddConnection(EGridDirection Direction, UMapTile* TileRef, EConnectionType ConnectionType, bool OverwriteExistingConnection)
{
	if (Direction == EGridDirection::NONE) return false;
	if (!IsValid(TileRef)) return false;
	int32 index = FindConnection(Direction);
	if (index != INDEX_NONE) {
		if (!OverwriteExistingConnection) {
			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("No Overwrite Allowed")));
			return false;
		}
		if (Connect[index].TileRef == TileRef) {
			return false;
		}
		if (IsValid(Connect[index].TileRef)) Connect[index].TileRef->UpdateConnection(FTileCoordinate::GetInverseDirection(Direction));
	}
	if(index == INDEX_NONE){
		index = Connect.Add(FConnection(Direction, TileRef, ConnectionType));
	}
	else {
		Connect[index].TileRef = TileRef;
		Connect[index].ConnectionType = ConnectionType;
	}
	TileRef->UpdateConnection(FTileCoordinate::GetInverseDirection(Direction), this, Connect[index].ConnectionType);
	return true;
}

bool UMapTile::AddConnectionInternal(EGridDirection Direction, UMapTile* TileRef, EConnectionType ConnectionType, bool OverwriteExistingConnection)
{
	if (Direction == EGridDirection::NONE) return false;
	int32 index = FindConnection(Direction, TileRef);
	if (index != INDEX_NONE) {
		if (!OverwriteExistingConnection) return false;
	}
	if (index == INDEX_NONE) {
		index = Connect.Add(FConnection(Direction, nullptr, ConnectionType));
	}
	return true;
}

bool UMapTile::RemoveConnection(EGridDirection Direction, UMapTile* TileRef)
{
	if(!IsValid(TileRef) && Direction == EGridDirection::NONE) return false;
	int32 index = INDEX_NONE;
	if (Direction != EGridDirection::NONE) {
		index = FindConnection(Direction);
	} else if (IsValid(TileRef)) {
		index = FindConnection(Direction, TileRef);
	}
	if(index == INDEX_NONE) return false;
	FConnection ConnectionToRemove = Connect[index];
	Connect.RemoveAt(index);
	ConnectionToRemove.TileRef->UpdateConnection(FTileCoordinate::GetInverseDirection(ConnectionToRemove.Direction));
    return true;
}

void UMapTile::UpdateConnection(EGridDirection Direction, UMapTile* TileRef, EConnectionType ConnectionType)
{
	// If neither parameter is valid, do nothing
	if (!IsValid(TileRef) && Direction == EGridDirection::NONE) return;
	
	//GEngine->AddOnScreenDebugMessage(-1,5,FColor::Red,"Updated...");

	if (IsValid(TileRef)) {
		AddConnectionInternal(Direction, TileRef, ConnectionType, true);
	}
	else {
		RemoveConnection(Direction);
	}
}

int32 UMapTile::FindConnection(EGridDirection Direction, UMapTile* TileRef) const
{
	int32 index = INDEX_NONE;
	//if (IsValid(TileRef)) {
		for (int32 i = 0; i < Connect.Num(); i++) {
			if (Connect[i].Direction != Direction) continue;
			return i;
			//break;
		}
	//}
	//else {
	//	for (int32 i = 0; i < Connect.Num(); i++) {
	//		if (Connect[i].TileRef == TileRef) {
	//			index = i;
	//			break;
	//		}
	//	}
	//}
	return index;
}
