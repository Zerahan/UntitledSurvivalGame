// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dev/MapGeneration/MapGeneratorTypes.h"
#include "DevMapGenerator_v2.generated.h"

USTRUCT(BlueprintType)
struct FConnection {
	GENERATED_BODY();

	FConnection() :
		Direction(EGridDirection::NONE),
		TileRef(nullptr),
		ConnectionType(EConnectionType::INVALID),
		IsOpen(true)
	{};

	//*
	FConnection(EGridDirection _Dir, UMapTile* _Tile, EConnectionType _Type) {
		Direction		= _Dir;
		TileRef			= _Tile;
		ConnectionType	= _Type;
		IsOpen			= false;
	}
	FConnection(EGridDirection _Dir, UMapTile* _Tile, EConnectionType _Type, bool _IsOpen) {
		Direction = _Dir;
		TileRef = _Tile;
		ConnectionType = _Type;
		IsOpen = _IsOpen;
	}
	//*/

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EGridDirection Direction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMapTile* TileRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EConnectionType ConnectionType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsOpen;

	bool IsValid() {
		return Direction != EGridDirection::NONE && ConnectionType != EConnectionType::INVALID && TileRef;
	}
};

UCLASS(BlueprintType)
class UNTITLEDSURVIVALGAME_API UMapTile : public UObject
{
	GENERATED_BODY()
	
public:
	UMapTile();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	uint8 ConnectedRoomPtr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Location;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FConnection> Connect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FConnection> Neighbors;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<EGridDirection> Doors;

	UPROPERTY(BlueprintReadOnly)
	TArray<bool> IsSharedShape;

	UPROPERTY(BlueprintReadOnly)
	int32 OriginalDoor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool HadConnectionRemoved;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTileShape LinkedShape;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D OriginalCoordinate;

public:
	UFUNCTION(BlueprintCallable)
	FVector GetWorldLocation(int32 TileSize = 400) const;

	UFUNCTION(BlueprintCallable)
	FVector GetRelativeLocation() const;

	UFUNCTION(BlueprintCallable)
	TArray<FConnection> GetConnections() const;

	UFUNCTION(BlueprintCallable)
	void SetOriginalCoordinate(FVector Vector);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CleanConnections(ADevMapGenerator_v2* GeneratorRef);
	virtual bool CleanConnections_Implementation(ADevMapGenerator_v2* GeneratorRef);

	UFUNCTION(BlueprintCallable)
	bool AddConnection(EGridDirection Direction, UMapTile* TileRef, EConnectionType ConnectionType = EConnectionType::SMALL, bool OverwriteExistingConnection = false);

	UFUNCTION(BlueprintCallable)
	bool AddConnectionInternal(EGridDirection Direction, UMapTile* TileRef = nullptr, EConnectionType ConnectionType = EConnectionType::SMALL, bool OverwriteExistingConnection = true);

	UFUNCTION(BlueprintCallable)
	bool RemoveConnection(EGridDirection Direction, UMapTile* TileRef = nullptr);

	UFUNCTION(BlueprintCallable)
	void UpdateConnection(EGridDirection Direction, UMapTile* TileRef = nullptr, EConnectionType ConnectionType = EConnectionType::INVALID);

	UFUNCTION(BlueprintCallable)
	int32 FindConnection(EGridDirection Direction, UMapTile* TileRef = nullptr) const;
};

UCLASS(BlueprintType, Blueprintable, Abstract)
class UNTITLEDSURVIVALGAME_API ADevMapGenerator_v2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADevMapGenerator_v2();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 DebugShapeIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 DebugDoorAtttachmentIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UMapTile*> Tiles;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FVector> BoundaryLocations;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FDoorInfo> BoundaryDoors;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FVector> FinishedLocations;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FDoorInfo> FinishedDoors;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FTileShape> TileShapes;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//TArray<UMapTile*> ClosedList;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 TileSize;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 WallThickness;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GenerateTileIterator();
	virtual bool GenerateTileIterator_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CleanTiles();
	virtual bool CleanTiles_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool FixPathing();
	virtual bool FixPathing_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PostBuildCleanup();
	virtual void PostBuildCleanup_Implementation();

	UFUNCTION(BlueprintCallable)
	void GetConnectedPaths(TArray<UMapTile*>& OpenList, TArray<UMapTile*>& CloseList);

	UFUNCTION(BlueprintCallable)
	bool CanMakeTileAt(FVector Location, FVector Offset = FVector::ZeroVector, FRotator Rotation = FRotator::ZeroRotator) const;

	//UFUNCTION(BlueprintCallable)
	//bool CanMakeTileAt(FVector Location, FVector& Offset, FRotator& Rotation) const;

	UFUNCTION(BlueprintCallable)
	UMapTile* MakeTile(FVector Location, FRotator Rotation = FRotator::ZeroRotator);

	UFUNCTION(BlueprintCallable)
	bool CanMakeTileShapeAt(FVector& LocationRef, FRotator& RotationRef, FDoorInfo& AlignmentDoor, int32& TileShapeIndex) const;
	
	UFUNCTION(BlueprintCallable)
	bool MakeTileShape(int32 BoundaryIndex, int32 TileShapeIndex, FVector Location, FRotator Rotation);

	UFUNCTION(BlueprintCallable)
	bool TileExistsAtLocation(FVector Location) const;

	UFUNCTION(BlueprintCallable)
	int32 GetTileAtLocation(FVector Location) const;

	UFUNCTION(BlueprintCallable)
	TArray<FTransform> GetAllConnectionLocations(TArray<bool>& IsOpen) const;

	UFUNCTION(BlueprintCallable)
	TArray<FTransform> GetAllShapes(TArray<FTileShape>& Shapes) const;

	void AddBoundaryLocation(FVector Location);

	UFUNCTION(BlueprintCallable)
	void DrawTile(UMapTile* Tile, FColor Color = FColor::White);

	UFUNCTION(BlueprintCallable)
	void DrawNormal(FVector Location, FRotator Rotation, FColor First = FColor::Red, FColor Second = FColor::Green, FColor Third = FColor::Blue);
	
	UFUNCTION(BlueprintCallable)
	void DrawConnection(FConnection Connection, UMapTile* ParentTile, FColor Color = FColor::White);

};
