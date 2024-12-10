// Fill out your copyright notice in the Description page of Project Settings.


#include "Dev/MapGeneration/MapGeneratorTypes.h"
#include "Engine/StaticMeshSocket.h"

TArray<FVector> FTileShape::GetContainedTileLocations() const
{
	return ContainedTileLocations;
}

void FTileShape::BuildTiles()
{
	ContainedTiles.Empty();
	//return;
	TArray<FVector> Neighbors;
	Neighbors.Add(FVector::ForwardVector);
	Neighbors.Add(FVector::RightVector);
	Neighbors.Add(FVector::ForwardVector * -1);
	Neighbors.Add(FVector::RightVector * -1);

	TArray<FVector> Doors;
	for (int32 i = 0; i < ContainedTileLocations.Num(); i++) {
		Doors.Empty();
		FTile Tile = FTile();
		Tile.Location = ContainedTileLocations[i];
		Tile.IsExternalEdge.Empty();
		Tile.ContainedDoors = GetDoorsOnTile(Tile.Location);
		for (int32 k = 0; k < Tile.ContainedDoors.Num(); k++) {
			Doors.Add(FTileCoordinate::GetDirectionAsVector(Tile.ContainedDoors[k].Direction));
		}
		for (int32 k = 0; k < Neighbors.Num(); k++) {
			Tile.IsExternalEdge.Add(!ContainedTileLocations.Contains(Tile.Location.AsVector() + Neighbors[k]));
			Tile.IsDoorEdge.Add(Doors.Contains(Neighbors[k]));
		}
		ContainedTiles.Add(Tile);
	}
}

void FTileShape::BuildEdges()
{
	AvailableEdges.Empty();

	TArray<FVector> NeighborOffsets;
	NeighborOffsets.Add(FVector(1, 0, 0));
	NeighborOffsets.Add(FVector(0, 1, 0));
	NeighborOffsets.Add(FVector(-1, 0, 0));
	NeighborOffsets.Add(FVector(0, -1, 0));

	TArray<FRotator> DirectionOffsets;
	DirectionOffsets.Add(FRotator(0, 0, 0));
	DirectionOffsets.Add(FRotator(0, 90, 0));
	DirectionOffsets.Add(FRotator(0, 180, 0));
	DirectionOffsets.Add(FRotator(0, 270, 0));
	
	FTransform Transform;
	for (const FVector Loc : ContainedTileLocations) {
		for (int32 i = 0; i < NeighborOffsets.Num(); i++){
			Transform.SetLocation(Loc + (NeighborOffsets[i] * 0.5));
			Transform.SetRotation(DirectionOffsets[i].Quaternion());
			if(IsContainedTile(Loc + NeighborOffsets[i])) AvailableEdges.Add(Transform);
		}
	}
}

void FTileShape::GetRandomEdge(FVector& Location, FRotator& Rotation)
{
	// Get a random tile within the shape.
	int32 index = FMath::RandRange(0, AvailableEdges.Num() - 1);
	Location = AvailableEdges[index].GetLocation();
	Rotation = AvailableEdges[index].GetRotation().Rotator();
}

bool FTileShape::IsContainedTile(FVector Location) const
{
	for (const FVector v : ContainedTileLocations) {
		if (v.Equals(Location)) return true;
	}
	return false;
}

void FTileShape::AlignToEdge(FTransform MyEdge, FTransform OtherEdge, FTransform& AlignmentTransform)
{
	AlignmentTransform.SetLocation(OtherEdge.GetLocation() + MyEdge.GetLocation());
	AlignmentTransform.SetRotation(OtherEdge.GetRotation() * MyEdge.GetRotation());
}

void FTileShape::AlignToEdge(FDoorInfo MyDoor, FDoorInfo OtherDoor, FTransform& AlignmentTransform)
{
	FRotator Rot = (FTileCoordinate::GetDirectionAsRotator(OtherDoor.Direction).Quaternion() * FTileCoordinate::GetDirectionAsRotator(MyDoor.Direction).Quaternion().Inverse()).Rotator();
	AlignmentTransform.SetLocation(OtherDoor.Location.AsVector() - Rot.RotateVector(MyDoor.Location.AsVector()));
	AlignmentTransform.SetRotation(Rot.Quaternion());
}

void FTileShape::DrawShape(UWorld* World, FTransform WorldPosition, float TileScale)
{
	if(ContainedTiles.Num() == 0) BuildTiles();
	
	FVector Origin = WorldPosition.GetLocation();
	FRotator Rotation = WorldPosition.GetRotation().Rotator();
	FVector Loc;
	FRotator Rot = (Rotation.Quaternion() * ShapeRotation.Quaternion()).Rotator();
	TArray<FVector> Points;
	Points.Add(FVector((FVector::ForwardVector)		 + (FVector::RightVector * -1)) * TileScale * 0.5f);
	Points.Add(FVector((FVector::ForwardVector)		 + (FVector::RightVector))		* TileScale * 0.5f);
	Points.Add(FVector((FVector::ForwardVector * -1) + (FVector::RightVector))		* TileScale * 0.5f);
	Points.Add(FVector((FVector::ForwardVector * -1) + (FVector::RightVector * -1)) * TileScale * 0.5f);

	for (int32 i = 0; i < ContainedTiles.Num(); i++) {
		Loc = Origin + Rot.RotateVector(ContainedTiles[i].Location.AsVector() * TileScale);
		//DrawDebugLine(World, Origin, Loc + FVector::UpVector * 10, FColor::White);
		if (ContainedTiles[i].IsExternalEdge[0]) DrawDebugLine(World, Loc + Rot.RotateVector(Points[0]), Loc + Rot.RotateVector(Points[1]), FColor::Blue); //(ContainedTiles[i].IsDoorEdge[0] ? FColor::Green : FColor::Blue));
		if (ContainedTiles[i].IsExternalEdge[1]) DrawDebugLine(World, Loc + Rot.RotateVector(Points[1]), Loc + Rot.RotateVector(Points[2]), FColor::Blue); //(ContainedTiles[i].IsDoorEdge[1] ? FColor::Green : FColor::Blue));
		if (ContainedTiles[i].IsExternalEdge[2]) DrawDebugLine(World, Loc + Rot.RotateVector(Points[2]), Loc + Rot.RotateVector(Points[3]), FColor::Blue); //(ContainedTiles[i].IsDoorEdge[2] ? FColor::Green : FColor::Blue));
		if (ContainedTiles[i].IsExternalEdge[3]) DrawDebugLine(World, Loc + Rot.RotateVector(Points[3]), Loc + Rot.RotateVector(Points[0]), FColor::Blue); //(ContainedTiles[i].IsDoorEdge[3] ? FColor::Green : FColor::Blue));
	}

	if (MeshRef->IsValidLowLevel()) {
		for(UStaticMeshSocket* Socket : MeshRef->Sockets){
			DrawDebugSphere(World, Origin + Rot.RotateVector(Socket->RelativeLocation), 16, 8, FColor::Red);
		}
	}

	/*/
	Points.Empty();
	Points.Add(FVector(FVector::RightVector * TileScale * -0.25));
	Points.Add(FVector(FVector::RightVector * TileScale * -0.25) + FVector::ForwardVector * -16);
	Points.Add(FVector(FVector::RightVector * TileScale * 0.25));
	Points.Add(FVector(FVector::RightVector * TileScale * 0.25) + FVector::ForwardVector * -16);
	FVector Offset;
	
	for (int32 i = 0; i < DoorInfo.Num(); i++) {
		Loc = Origin + Rot.RotateVector(DoorInfo[i].Location.AsVector() * TileScale);
		Offset = FTileCoordinate::GetDirectionAsVector(DoorInfo[i].Direction);
		DoorInfo[i].Draw(World, 1, Loc, Rotation);
		if(DoorInfo[i].Direction != EGridDirection::NONE) DrawDebugDirectionalArrow(World, Loc + Rot.RotateVector(Offset * TileScale * 0.25), Loc + Rot.RotateVector(FTileCoordinate::GetDirectionAsVector(DoorInfo[i].Direction) * TileScale * 0.5), 64, FColor::Green);
		DrawDebugLine(World, Loc + (Rot.RotateVector(Offset) * TileScale * 0.5) + Rot.RotateVector(Offset.Rotation().RotateVector(Points[0])), Loc + (Rot.RotateVector(Offset) * TileScale * 0.5) + Rot.RotateVector(Offset.Rotation().RotateVector(Points[1])), FColor::White);
		DrawDebugLine(World, Loc + (Rot.RotateVector(Offset) * TileScale * 0.5) + Rot.RotateVector(Offset.Rotation().RotateVector(Points[2])), Loc + (Rot.RotateVector(Offset) * TileScale * 0.5) + Rot.RotateVector(Offset.Rotation().RotateVector(Points[3])), FColor::White);
		DrawDebugLine(World, Loc + (Rot.RotateVector(Offset) * TileScale * 0.5) + Rot.RotateVector(Offset.Rotation().RotateVector(Points[1])), Loc + (Rot.RotateVector(Offset) * TileScale * 0.5) + Rot.RotateVector(Offset.Rotation().RotateVector(Points[3])), FColor::White);
	}
	//*/
}

TArray<FDoorInfo> FTileShape::GetDoorsOnTile(FTileCoordinate Location) const
{
	TArray<FDoorInfo> Doors;
	for (int32 i = 0; i < DoorInfo.Num(); i++) {
		if(DoorInfo[i].Location.Equals(Location)) Doors.Add(DoorInfo[i]);
	}
	return Doors;
}

inline EGridDirection FTileCoordinate::GetInverseDirection(EGridDirection Direction) {
	switch (Direction) {
	case EGridDirection::N:  return EGridDirection::S;
	case EGridDirection::NE: return EGridDirection::SW;
	case EGridDirection::E:  return EGridDirection::W;
	case EGridDirection::SE: return EGridDirection::NW;
	case EGridDirection::S:  return EGridDirection::N;
	case EGridDirection::SW: return EGridDirection::NE;
	case EGridDirection::W:  return EGridDirection::E;
	case EGridDirection::NW: return EGridDirection::SE;
	case EGridDirection::U:  return EGridDirection::D;
	case EGridDirection::D:  return EGridDirection::U;
	default: return EGridDirection::NONE;
	}
}

inline FVector FTileCoordinate::GetDirectionAsVector(EGridDirection Direction, bool Normalize) {
	switch (Direction) {
		case EGridDirection::N: return FVector::ForwardVector;
		case EGridDirection::E: return FVector::RightVector;
		case EGridDirection::S: return FVector::ForwardVector * -1;
		case EGridDirection::W: return FVector::RightVector * -1;

		case EGridDirection::U:  return FVector::UpVector;
		case EGridDirection::D:  return FVector::UpVector * -1;
	}
	if (Normalize) {
		switch (Direction) {
			case EGridDirection::NE: return (FVector::ForwardVector + FVector::RightVector).GetUnsafeNormal();
			case EGridDirection::SE: return ((FVector::ForwardVector * -1) + FVector::RightVector).GetUnsafeNormal();
			case EGridDirection::SW: return ((FVector::ForwardVector * -1) + (FVector::RightVector * -1)).GetUnsafeNormal();
			case EGridDirection::NW: return ((FVector::ForwardVector)+(FVector::RightVector * -1)).GetUnsafeNormal();
			default:				 return FVector::ZeroVector;
		}
	}
	else {
		switch (Direction) {
			case EGridDirection::NE: return FVector::ForwardVector + FVector::RightVector;
			case EGridDirection::SE: return (FVector::ForwardVector * -1) + FVector::RightVector;
			case EGridDirection::SW: return (FVector::ForwardVector * -1) + (FVector::RightVector * -1);
			case EGridDirection::NW: return (FVector::ForwardVector)+(FVector::RightVector * -1);
			default:				 return FVector::ZeroVector;
		}
	}
}

inline FRotator FTileCoordinate::GetDirectionAsRotator(EGridDirection Direction)
{
	switch (Direction) {
		case EGridDirection::N: return FRotator(0, 0, 0);
		case EGridDirection::E: return FRotator(0, 90, 0);
		case EGridDirection::S: return FRotator(0, -180, 0);
		case EGridDirection::W: return FRotator(0, -90, 0);

		case EGridDirection::U:  return FRotator(90, 0, 0);
		case EGridDirection::D:  return FRotator(-90, 0, 0);

		case EGridDirection::NE: return FRotator(0, 45, 0);
		case EGridDirection::SE: return FRotator(0, 135, 0);
		case EGridDirection::SW: return FRotator(0, -135, 0);
		case EGridDirection::NW: return FRotator(0, -45, 0);
		default:				 return FRotator::ZeroRotator;
	}
}

inline EGridDirection FTileCoordinate::GetRotatorAsDirection(FRotator Rot) {
	float RawYaw;
	if (Rot.Yaw < 0) {
		RawYaw = FMath::Abs((int)Rot.Yaw) % 360 * -1;
		if(RawYaw < -180) RawYaw += 360;
	}
	else {
		RawYaw = (int)Rot.Yaw % 360;
		if(RawYaw > 180) RawYaw -=360;
	}
	int32 Yaw = FMath::RoundToInt(RawYaw / 45.f);
	switch (Yaw) {
		case -4: return EGridDirection::S;
		case -3: return EGridDirection::SW;
		case -2: return EGridDirection::W;
		case -1: return EGridDirection::NW;
		case 0:  return EGridDirection::N;
		case 1:  return EGridDirection::NE;
		case 2:  return EGridDirection::E;
		case 3:  return EGridDirection::SE;
		case 4: return EGridDirection::S;
		default:
			int32 Sign = FMath::Sign(Rot.Pitch);
			int32 Pitch = FMath::CeilToInt(FMath::Abs(Rot.Pitch) / 90) * Sign;
			switch (Sign) {
				case -1:	return EGridDirection::D;
				case 1:		return EGridDirection::U;
				default:	return EGridDirection::NONE;
			}
	}
	//return EGridDirection::NONE;
}

inline FDoorInfo FDoorInfo::GetInverse() const {
	FDoorInfo Info;
	Info.Location = FTileCoordinate(Location.AsVector() + FTileCoordinate::GetDirectionAsVector(Direction, false));
	Info.Direction = FTileCoordinate::GetInverseDirection(Direction);
	Info.Type = Type;
	return Info;
}

inline void FDoorInfo::Draw(UWorld* World, const int32 ParentScale, const FVector ParentLocation, const FRotator ParentRotation, const FColor Color, const int32 Offset) const {
	FVector Dir = FTileCoordinate::GetDirectionAsVector(Direction);
	FVector Loc = ParentLocation + ParentRotation.RotateVector(Location.AsVector() * ParentScale);
	DrawDebugBox(World, Loc + (Dir * 50) + (FVector::UpVector * Offset * 0.5), FVector(10, 10, 10 + Offset), Color);
	DrawDebugDirectionalArrow(World, Loc + (Dir * 50) + (FVector::UpVector * Offset), Loc + (Dir * 100) + (FVector::UpVector * Offset), 30, Color);
	FText EnumName;
	UEnum::GetDisplayValueAsText(Direction, EnumName);
	DrawDebugString(World, Loc + (Dir * 50) + (FVector::UpVector * Offset), EnumName.ToString() + "\n" + Location.ToString(), 0, Color);
}
