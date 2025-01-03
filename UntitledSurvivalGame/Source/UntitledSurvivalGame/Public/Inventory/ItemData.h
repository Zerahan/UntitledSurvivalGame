// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemData.generated.h"

class UItemAsset_Generic;
class UInventoryComponent;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UNTITLEDSURVIVALGAME_API UItemData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	TSubclassOf<UItemAsset_Generic> ClassRef;
	
public:
	UItemData()
		: ClassRef(nullptr)
		{};

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetAmount() const;
	virtual int32 GetAmount_Implementation() const;
	
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UItemAsset_Generic> GetClassRef() const;

	UFUNCTION(BlueprintCallable)
	UItemAsset_Generic* GetClassDefaultObject() const;

	UFUNCTION(BlueprintCallable)
	UItemAsset_Generic* GetStaticData() const;

	UFUNCTION(BlueprintCallable)
	UInventoryComponent* GetOwningInventory() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsItemValid() const;
	virtual bool IsItemValid_Implementation() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanMerge(UItemData* DataToMerge) const;
	virtual bool CanMerge_Implementation(UItemData* DataToMerge) const;

	UFUNCTION(BlueprintCallable)
	bool MergeData(UItemData* DataToMerge);
	
protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool MergeData_Internal(UItemData* DataToMerge);
	virtual bool MergeData_Internal_Implementation(UItemData* DataToMerge);
};
