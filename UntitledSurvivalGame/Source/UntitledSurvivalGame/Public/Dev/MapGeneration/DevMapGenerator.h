// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dev/MapGeneration/MapGeneratorTypes.h"
#include "DevMapGenerator.generated.h"


UENUM(BlueprintType)
enum class EDoorSize : uint8 {
	Medium	= 0 UMETA(DisplayName = "Medium"),
	Open	= 1 << 0 UMETA(DisplayName = "Open"),
	Small	= 1 << 1 UMETA(DisplayName = "Small"),
	Large	= 1 << 2 UMETA(DisplayName = "Large"),
};
ENUM_CLASS_FLAGS(EDoorSize);

USTRUCT(BlueprintType)
struct FDoorTile {
	GENERATED_BODY();

	FDoorTile():
		Location(FVector::ZeroVector),
		//Normal(FVector::ForwardVector),
		Rotation(FRotator::ZeroRotator),
		DoorSize(0)
	{};
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Location;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//FVector Normal;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator Rotation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	uint8 DoorSize;
};

USTRUCT(BlueprintType)
struct FRoomTile {
	GENERATED_BODY();

	FRoomTile():
		Location(FVector::ZeroVector),
		Rotation(FRotator::ZeroRotator),
		MeshIndex(-1),
		InstanceIndex(-1),
		PrimaryDoorIndex(-1)
	{};
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Location;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator Rotation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MeshIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 InstanceIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 PrimaryDoorIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FDoorTile> Doors;

	void AlignToTransform(FTransform Transform);

	TArray<FDoorTile> GetWorldAlignedDoors(AActor* ParentActor);

	bool IsValid(){ return Location != FVector::ZeroVector && PrimaryDoorIndex != -1; };
};

USTRUCT(BlueprintType)
struct FMeshData {
	GENERATED_BODY();

	FMeshData(){};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FDoorTile> Doors;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<uint8> Sizes;
};


class UHierarchicalInstancedStaticMeshComponent;
class UBoxComponent;

UCLASS(BlueprintType, Blueprintable, Abstract)
class UNTITLEDSURVIVALGAME_API ADevMapGenerator : public AActor
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionTester;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<FMeshData> MeshesToUse;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<UHierarchicalInstancedStaticMeshComponent*> InstancedMeshes;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<FDoorTile> Doors;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<FDoorTile> CompletedDoors;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<FRoomTile> Rooms;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool IsFinished;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	int32 TileSize;
	
public:
	// Sets default values for this actor's properties
	ADevMapGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	void GenerateMap_Iterator();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GenerateNextRoom();
	virtual bool GenerateNextRoom_Implementation();

	UFUNCTION(BlueprintCallable)
	void DrawDoor(FDoorTile Door, FVector ParentLocation, FRotator ParentRotation, FColor Color = FColor::White);

	UFUNCTION(BlueprintCallable)
	void DrawRoom(FRoomTile Room, FColor Color = FColor::White);

	UFUNCTION(BlueprintCallable)
	void DrawNormal(FVector Location, FRotator Rotation, FColor First = FColor::Red, FColor Second = FColor:: Green, FColor Third = FColor::Blue);


	UFUNCTION(BlueprintCallable)
	void AlignRoomToTransform(int32 RoomIndex, FTransform Transform = FTransform());
};
