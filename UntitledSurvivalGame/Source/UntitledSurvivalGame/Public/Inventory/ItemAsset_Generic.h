// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemAsset_Generic.generated.h"

class ASpawnedItem_Generic;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UNTITLEDSURVIVALGAME_API UItemAsset_Generic : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UItemAsset_Generic();
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FText DisplayName;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<ASpawnedItem_Generic> SpawnedItem;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSoftObjectPtr<UStaticMesh> SpawnedMesh;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool CanMerge;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 MaxAmount;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool IsUsable;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool OnUse(APawn* InstigatingPlayerPawn);
	virtual bool OnUse_Implementation(APawn* InstigatingPlayerPawn);
};
