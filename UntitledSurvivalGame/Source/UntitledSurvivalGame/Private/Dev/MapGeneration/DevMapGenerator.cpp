// Fill out your copyright notice in the Description page of Project Settings.


#include "Dev/MapGeneration/DevMapGenerator.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/BoxComponent.h"
#if WITH_EDITOR
	#include "DrawDebugHelpers.h"
#endif

// Sets default values
ADevMapGenerator::ADevMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TileSize = 200;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default Root Component"));
	
	CollisionTester = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapTester"));
	if (CollisionTester) {
		CollisionTester->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
		CollisionTester->SetBoxExtent(FVector(199, 199, 149));
		CollisionTester->SetRelativeLocation(FVector(0, 0, -1000));
	}
}

// Called when the game starts or when spawned
void ADevMapGenerator::BeginPlay()
{
	Super::BeginPlay();

	UHierarchicalInstancedStaticMeshComponent* Component;

	for(FMeshData Mesh : MeshesToUse) {
		for (FDoorTile Door : Mesh.Doors) {
			Mesh.Sizes.AddUnique(Door.DoorSize);
		}
		Component = Cast< UHierarchicalInstancedStaticMeshComponent>(AddComponentByClass(UHierarchicalInstancedStaticMeshComponent::StaticClass(), false, FTransform(), false));
		if (Component) {
			Component->SetStaticMesh(Mesh.Mesh.LoadSynchronous());
			Component->AttachToComponent(RootComponent,FAttachmentTransformRules::SnapToTargetIncludingScale);
			Component->bMultiBodyOverlap = true;
			InstancedMeshes.Add(Component);
		}
	}
	
	//Doors.Add(FDoorTile());
	FDoorTile Tile;
	Tile.Location = FVector(0,0,0);
	Tile.Rotation = FRotator::ZeroRotator;
	Tile.DoorSize = 1;
	Doors.Add(Tile);
	IsFinished = false;
	GenerateMap_Iterator();
}

void ADevMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GenerateMap_Iterator();
#if WITH_EDITOR
	for (const FDoorTile tile : Doors) {
		//DrawDoor(tile, FVector(), GetActorRotation(), FColor::White);
		FVector Loc = GetActorLocation() + GetActorRotation().RotateVector(tile.Location + (FVector::UpVector * 50));
		DrawDebugSphere(GetWorld(), Loc, 16, 8, FColor::White);
		DrawNormal(Loc, (GetActorRotation().Quaternion() * tile.Rotation.Quaternion()).Rotator());
		//DrawDebugDirectionalArrow(GetWorld(), tile.Location + (FVector::UpVector * 50), tile.Location + (FVector::UpVector * 50) + tile.Normal * 50, 20, FColor::Cyan, false, -1.f, 0, 2.f);
	}
	for (const FDoorTile tile : CompletedDoors) {
		DrawDoor(tile, GetActorLocation(), GetActorRotation(), FColor::Cyan);
	}
	for (const FRoomTile tile : Rooms) {
		DrawRoom(tile);
	}
#endif
}

void ADevMapGenerator::GenerateMap_Iterator()
{
	if(IsFinished) return;
	if (!GenerateNextRoom()) {
		IsFinished = true;
		return;
	}
}

bool ADevMapGenerator::GenerateNextRoom_Implementation()
{
	if (Doors.Num() == 0) return false;
	if (CompletedDoors.Num() > 10) { return false; }
	if (InstancedMeshes.Num() == 0) { return false; }
	if (MeshesToUse.Num() == 0) { return false; }
	FDoorTile NextDoor;
	int32 index = FMath::RandRange(0, Doors.Num() - 1);
	if (!Doors.IsValidIndex(index)) index = 0;
	NextDoor = Doors[index];
	Doors.RemoveAt(index);
	CompletedDoors.Add(NextDoor);

	int32 randIndex = FMath::RandRange(0, InstancedMeshes.Num() - 1);
	if (!InstancedMeshes.IsValidIndex(randIndex)) randIndex = 0;

	// TODO Verify the selected mesh has a proper-sized door available.
	// TODO Verify the selected mesh can fit in the space
	// TODO Verify the select mesh can plug into the neighboring door slots as well.
	CollisionTester->SetRelativeLocation(NextDoor.Location + NextDoor.Rotation.Vector() * 200 + FVector::UpVector * 75);
	if (CollisionTester->IsOverlappingActor(this)) {
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "GenerateMap: Overlap Detected! Moving on...");
		CollisionTester->SetRelativeLocation(FVector(0, 0, -1000));
		return false;
	}



	TArray<FDoorTile> MeshDoors = MeshesToUse[randIndex].Doors;
	int32 DoorToUse = -1;
	bool FoundDoorToUse = false;
	TArray<uint8> TrimmedList;
	for (int32 i = 0; i < MeshDoors.Num(); i++) {
		if (MeshDoors[i].DoorSize == NextDoor.DoorSize) {
			TrimmedList.Add(i);
		}
	}
	if (TrimmedList.Num() == 0) {
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "GenerateMap: Error: No doors on TrimmedList!");
		return false;
	}
	DoorToUse = TrimmedList[FMath::RandRange(0,TrimmedList.Num()-1)];
	FoundDoorToUse = TrimmedList.Num() > 0;

	// TODO Add the unattached doors to the doors list for continued map generation.
	if (FoundDoorToUse) {
		FTransform Transform;
		Transform.SetRotation(MeshDoors[DoorToUse].Rotation.Quaternion() * NextDoor.Rotation.Quaternion());
		Transform.SetLocation(MeshDoors[DoorToUse].Location + NextDoor.Location);
		FRoomTile Room;
		Room.MeshIndex		= randIndex;
		Room.Doors			= MeshDoors;
		Room.PrimaryDoorIndex	= DoorToUse;
		Room.AlignToTransform(Transform);
		TArray<FDoorTile> AlignedDoors = Room.GetWorldAlignedDoors(this);
		for (int32 i = 0; i < AlignedDoors.Num(); i++) {
			if (Room.PrimaryDoorIndex != i) {
				Doors.Add(AlignedDoors[i]);
			}
		}

		Transform.SetRotation(Room.Rotation.Quaternion()); // * GetActorRotation().Quaternion()
		Transform.SetLocation(Room.Location);
		Room.InstanceIndex = InstancedMeshes[randIndex]->AddInstance(Transform, false);
		Rooms.Add(Room);
	}

	return true;
}

void ADevMapGenerator::DrawDoor(FDoorTile Door, FVector ParentLocation, FRotator ParentRotation, FColor Color)
{
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
}

void ADevMapGenerator::DrawRoom(FRoomTile Room, FColor Color)
{
	FVector Loc = GetActorLocation() + GetActorRotation().RotateVector(Room.Location);
	FQuat Rot = GetActorRotation().Quaternion() * Room.Rotation.Quaternion();
	bool DrawDoors = false;
	DrawDebugLine(GetWorld(), Loc + GetActorRotation().RotateVector(FVector(0 - TileSize, 0 - TileSize, 0)), Loc + GetActorRotation().RotateVector(FVector(TileSize, 0 - TileSize, 0)), FColor::Blue);
	DrawDebugLine(GetWorld(), Loc + GetActorRotation().RotateVector(FVector(TileSize, 0 - TileSize, 0)), Loc + GetActorRotation().RotateVector(FVector(TileSize, TileSize, 0)), FColor::Blue);
	DrawDebugLine(GetWorld(), Loc + GetActorRotation().RotateVector(FVector(TileSize, TileSize, 0)), Loc + GetActorRotation().RotateVector(FVector(0 - TileSize, TileSize, 0)), FColor::Blue);
	DrawDebugLine(GetWorld(), Loc + GetActorRotation().RotateVector(FVector(0 - TileSize, TileSize, 0)), Loc + GetActorRotation().RotateVector(FVector(0 - TileSize, 0 - TileSize, 0)), FColor::Blue);
	//DrawDebugBox(GetWorld(), Room.Location, FVector(200, 200, 1), FColor::Blue);
	//DrawDebugDirectionalArrow(GetWorld(), Room.Location + GetActorRotation().RotateVector(FVector::UpVector * 10), Room.Location + GetActorRotation().RotateVector(FVector::UpVector*10 + Room.Rotation.Vector() * 50), 20, FColor::Red);
	DrawNormal(Loc + GetActorRotation().RotateVector(FVector::UpVector * 10), Rot.Rotator());
	if (DrawDoors) {
		for (int32 i = 0; i < Room.Doors.Num(); i++) {
			if (Room.PrimaryDoorIndex == i) {
				DrawDebugLine(GetWorld(), Loc, Loc + Rot.RotateVector(Room.Doors[i].Location), FColor::Red);
			}
			DrawDoor(Room.Doors[i], Loc, Rot.Rotator(), (Room.PrimaryDoorIndex == i ? FColor::Red : FColor::White));
		}
	}
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (Room.Rotation.Vector() * 50), FColor::Purple);
	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (Room.Rotation.RotateVector(Room.Doors[Room.PrimaryDoorIndex].Normal) * 40), FColor::Cyan);
}

void ADevMapGenerator::DrawNormal(FVector Location, FRotator Rotation, FColor First, FColor Second, FColor Third)
{
	DrawDebugDirectionalArrow(GetWorld(), Location, Location + (Rotation.RotateVector(FVector::ForwardVector) * 50), 20, First);
	DrawDebugDirectionalArrow(GetWorld(), Location, Location + (Rotation.RotateVector(FVector::RightVector) * 30), 20, Second);
	DrawDebugDirectionalArrow(GetWorld(), Location, Location + (Rotation.RotateVector(FVector::UpVector) * 30), 20, Third);
}

void ADevMapGenerator::AlignRoomToTransform(int32 RoomIndex, FTransform Transform)
{
	if(!Rooms.IsValidIndex(RoomIndex)) return;
	//if(!Rooms[RoomIndex].IsValid()) return;
	Rooms[RoomIndex].AlignToTransform(Transform);
	//Rooms[RoomIndex].Rotation = (GetActorRotation().Quaternion() * Rooms[RoomIndex].Rotation.Quaternion()).Rotator();
}

void FRoomTile::AlignToTransform(FTransform Transform)
{
	Rotation = (Transform.GetRotation().Vector() * -1.f).Rotation().GetInverse();
	Location = (Transform.GetRotation().Rotator().GetInverse()).RotateVector(Transform.GetLocation());

	for (FDoorTile Door : Doors) {
		Door.Location = Location + Rotation.RotateVector(Door.Location);
		Door.Rotation = (Rotation.Quaternion() * Door.Rotation.Quaternion()).Rotator();
	}
}

TArray<FDoorTile> FRoomTile::GetWorldAlignedDoors(AActor* ParentActor) {
	FVector Loc = Location; //ParentActor->GetActorLocation() + ParentActor->GetActorRotation().RotateVector(Location);
	FQuat Rot = Rotation.Quaternion(); //ParentActor->GetActorRotation().Quaternion() * Rotation.Quaternion();
	TArray<FDoorTile> AlignedDoors;
	FDoorTile Tile;
	for (int32 i = 0; i < Doors.Num(); i++) {
		Tile.DoorSize = Doors[i].DoorSize;
		Tile.Location = Loc + Rot.RotateVector(Doors[i].Location);
		Tile.Rotation = (Doors[i].Rotation.Quaternion() * Rot).Rotator();
		AlignedDoors.Add(Tile);
	}
	return AlignedDoors;
}
