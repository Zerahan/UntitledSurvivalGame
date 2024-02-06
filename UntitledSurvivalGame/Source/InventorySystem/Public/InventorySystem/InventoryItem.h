// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItem.generated.h"

class UItemDataAsset;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class INVENTORYSYSTEM_API UInventoryItem : public UObject
{
	GENERATED_BODY()
	
private:
	UInventoryItem();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UItemDataAsset* ItemType;

public:
	UFUNCTION(BlueprintCallable)
	UTexture2D* GetIcon() const;

	UFUNCTION(BlueprintCallable)
	UItemDataAsset* GetItemType() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanMerge(UInventoryItem* ItemToMerge) const;
	virtual bool CanMerge_Implementation(UInventoryItem* ItemToMerge) const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DoMerge(UInventoryItem* ItemToMerge);
	virtual void DoMerge_Implementation(UInventoryItem* ItemToMerge);
};
