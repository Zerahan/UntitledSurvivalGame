// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dev/MapGeneration/DevMapGenerator.h"
#include "AlignmentHelper.generated.h"

class UArrowComponent;

UCLASS(BlueprintType)
class UNTITLEDSURVIVALGAME_API AAlignmentHelper : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAlignmentHelper();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true", MakeEditWidget = true))
	FVector OriginPoint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FRotator Direction;

//#if WITH_EDITOR
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<FDoorTile> DoorTiles;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FTileShape TileShape;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 TileSize;

	public:
	UFUNCTION(BlueprintCallable)
	FTileShape GetTileShape() const;
//#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	UFUNCTION(BlueprintCallable)
	void RebuildShape();

};
