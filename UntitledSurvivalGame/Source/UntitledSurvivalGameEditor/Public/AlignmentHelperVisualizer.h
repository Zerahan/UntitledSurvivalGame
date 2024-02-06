// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComponentVisualizer.h"

/**
 * 
 */
class UNTITLEDSURVIVALGAMEEDITOR_API FAlignmentHelperVisualizer : public FComponentVisualizer
{
public:
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
	
	void GetDoorPoints(uint8 DoorSize, TArray<FVector>& Points) const;

	void GetTilePoints(int32 TileSize, TArray<FVector>& Points) const;
};
