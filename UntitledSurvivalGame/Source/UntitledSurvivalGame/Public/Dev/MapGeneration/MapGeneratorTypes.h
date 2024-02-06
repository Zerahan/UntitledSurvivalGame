// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapGeneratorTypes.generated.h"


UENUM(BlueprintType)
enum class EConnectionType : uint8 {
	INVALID UMETA(DisplayName = "Invalid"),
	FULL	UMETA(DisplayName = "No Wall"),
	SMALL	UMETA(DisplayName = "Small Door"),
	MEDIUM	UMETA(DisplayName = "Medium Door"),
	LARGE	UMETA(DisplayName = "Large Door"),
	TINY	UMETA(DisplayName = "Vent"),
};


UENUM(BlueprintType)
enum class EGridDirection : uint8 {
	NONE	UMETA(DisplayName = "Unknown"),
	N	UMETA(DisplayName = "North"),
	NE	UMETA(DisplayName = "North East"),
	E	UMETA(DisplayName = "East"),
	SE	UMETA(DisplayName = "South East"),
	S	UMETA(DisplayName = "South"),
	SW	UMETA(DisplayName = "South West"),
	W	UMETA(DisplayName = "West"),
	NW	UMETA(DisplayName = "North West"),
	U	UMETA(DisplayName = "Up"),
	D	UMETA(DisplayName = "Down")
};

USTRUCT(BlueprintType)
struct UNTITLEDSURVIVALGAME_API FTileCoordinate {
	GENERATED_BODY()

	FTileCoordinate()
		: X(0)
		, Y(0)
		, Z(0)
	{};

	FTileCoordinate(FVector V) {
		X = V.X;
		Y = V.Y;
		Z = V.Z;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 X;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Y;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Z;
	
	bool operator==(const FTileCoordinate& Other) const { return Other.X == X && Other.Y == Y && Other.Z == Z; }
	bool operator==(const FVector& Other) const { return  Other.Equals(AsVector()); }
	bool operator!=(const FTileCoordinate& Other) const { return !(*this == Other); }
	bool operator!=(const FVector& Other) const { return !(*this == Other); }

	FTileCoordinate& operator+(const FTileCoordinate& B) { X += B.X; Y += B.Y; Z += B.Z; return *this; }
	FTileCoordinate& operator+(const FVector& B) { X += B.X; Y += B.Y; Z += B.Z; return *this; }
	FTileCoordinate& operator-(const FTileCoordinate& B) { X -= B.X; Y -= B.Y; Z -= B.Z; return *this; }
	FTileCoordinate& operator-(const FVector& B) { X -= B.X; Y -= B.Y; Z -= B.Z; return *this; }

	bool Equals(FTileCoordinate& Other) const { return (*this == Other); }
	bool Equals(FVector& Other) const { return (*this == Other); }

	//bool Equals(FTileCoordinate Other) { return (Other.X == X && Other.Y == Y && Other.Z == Z); }
	//bool Equals(FVector Other) { return (Other.Equals(AsVector())); }

	FVector AsVector() const {
		return FVector(X, Y, Z);
	}

	FString ToString() const {
		return FString::Printf(TEXT("X: %i, Y: %i, Z: %i"), X, Y, Z);
	}

	static EGridDirection GetInverseDirection(EGridDirection Direction);

	static FVector GetDirectionAsVector(EGridDirection Direction, bool Normalize = false);
	
	static FRotator GetDirectionAsRotator(EGridDirection Direction);

	static EGridDirection GetRandomDirection() {
		int32 index = FMath::RandRange(0,3);
		switch (index) {
			case 0: return EGridDirection::N;
			case 1: return EGridDirection::E;
			case 2: return EGridDirection::S;
			case 3: return EGridDirection::W;
			default: return EGridDirection::N;
		}
		return EGridDirection::N;
	}

	static EGridDirection GetRotatorAsDirection(FRotator Rot);

	static EGridDirection RotateDirectionByRotator(EGridDirection Source, FRotator Rot) {
		return GetRotatorAsDirection(Rot.RotateVector(GetDirectionAsVector(Source)).Rotation());
	}
};

USTRUCT(BlueprintType)
struct UNTITLEDSURVIVALGAME_API FDoorInfo
{
	GENERATED_BODY()

	FDoorInfo()
		: Location(FTileCoordinate())
		, Direction(EGridDirection::NONE)
		, Type(EConnectionType::INVALID)
	{};

	FDoorInfo(FVector _Location, EGridDirection _Direction, EConnectionType _Type)
	{
		Location	= FTileCoordinate(_Location);
		Direction	= _Direction;
		Type		= _Type;
	};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTileCoordinate Location;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EGridDirection Direction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EConnectionType Type;

	bool operator==(const FDoorInfo& Other) const { return Location == Other.Location && Direction == Other.Direction && Type == Other.Type;	}
	bool operator!=(const FDoorInfo& Other) const { return !(*this==Other); }

	FDoorInfo GetInverse() const;

	void Draw(UWorld* World, const int32 ParentScale, const FVector ParentLocation, const FRotator ParentRotation, const FColor Color = FColor::White, const int32 Offset = 0) const;
};

USTRUCT(BlueprintType)
struct UNTITLEDSURVIVALGAME_API FTile {
	GENERATED_BODY()

	FTile()
		: Location(FVector::ZeroVector)
	{};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTileCoordinate Location;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FDoorInfo> ContainedDoors;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<bool> IsExternalEdge;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<bool> IsDoorEdge;
};

USTRUCT(BlueprintType)
struct UNTITLEDSURVIVALGAME_API FTileShape
{
	GENERATED_BODY()

	FTileShape()
		: IsValidDrawTarget(false)
	{};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FVector> ContainedTileLocations;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FDoorInfo> DoorInfo;

	UPROPERTY(BlueprintReadOnly)
	FRotator ShapeRotation;

	UPROPERTY(BlueprintReadOnly)
	TArray<FTile> ContainedTiles;

	UPROPERTY(BlueprintReadOnly)
	TArray<FTransform> AvailableEdges;

	UPROPERTY(BlueprintReadOnly)
	bool IsValidDrawTarget;

	TArray<FVector> GetContainedTileLocations() const;

	//UFUNCTION(BlueprintCallable)
	void BuildTiles();

	//UFUNCTION(BlueprintCallable)
	void BuildEdges();

	//UFUNCTION(BlueprintCallable)
	void GetRandomEdge(FVector& Location, FRotator& Rotation);

	//UFUNCTION(BlueprintCallable)
	bool IsContainedTile(FVector Location) const;

	//UFUNCTION(BlueprintCallable)
	void AlignToEdge(FTransform MyEdge, FTransform OtherEdge, FTransform& AlignmentTransform);

	//UFUNCTION(BlueprintCallable)
	void AlignToEdge(FDoorInfo MyDoor, FDoorInfo OtherDoor, FTransform& AlignmentTransform);

	//UFUNCTION(BlueprintCallable)
	void DrawShape(UWorld* World, FTransform WorldPosition, float TileScale);

	TArray<FDoorInfo> GetDoorsOnTile(FTileCoordinate Coord) const;
};