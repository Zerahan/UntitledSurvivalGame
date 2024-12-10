// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/ItemData.h"
#include "ItemData_Quantity.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UNTITLEDSURVIVALGAME_API UItemData_Quantity : public UItemData
{
	GENERATED_BODY()
	
public:
	UItemData_Quantity()
		: Amount(0)
	{};
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Amount;

	virtual int32 GetAmount_Implementation() const override;
	
	virtual bool IsItemValid_Implementation() const override;

	virtual bool CanMerge_Implementation(UItemData* DataToMerge) const override;

	virtual bool MergeData_Internal_Implementation(UItemData* DataToMerge) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool AddAmount(int32& Value);
	virtual bool AddAmount_Implementation(int32& Value);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool RemoveAmount(int32& Value);
	virtual bool RemoveAmount_Implementation(int32& Value);
};
