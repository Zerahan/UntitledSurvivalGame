// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Dev/MapGeneration/DevMapGenerator.h"
#include "AlignmentHelperComponent.generated.h"

class AAlignmentHelper;

UCLASS( BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTITLEDSURVIVALGAME_API UAlignmentHelperComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAlignmentHelperComponent();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FDoorTile> DoorTiles;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	int32 GetNumDoors() const;

	UFUNCTION(BlueprintCallable)
	bool GetDoor(int32 index, FVector& Location, FRotator& Rotation, uint8& Size) const;

	UFUNCTION(BlueprintCallable)
	FTileShape GetShape() const;

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetContainedTiles() const;

	UFUNCTION(BlueprintCallable)
	int32 GetTileSize() const;
};
