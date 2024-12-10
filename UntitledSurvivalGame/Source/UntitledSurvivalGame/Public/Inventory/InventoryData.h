// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryData.generated.h"

class UItemData;
class UItemAsset_Generic;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventorySlotUpdated, int32, Index, int32, Slot);

/**
 * 
 */
UCLASS(BlueprintType)
class UNTITLEDSURVIVALGAME_API UInventoryData : public UObject
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	int32 InventorySize;
	
	UPROPERTY()
	int32 DataIndex;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	FString DataName;

public:
	UInventoryData();

	UFUNCTION()
	void Initialize(int32 Index, int32 Size);

	UFUNCTION(BlueprintCallable)
	void SetDataName(FString NewName);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UItemData*> Slots;

public:
	UFUNCTION(BlueprintCallable)
	int32 GetDataIndex() const;

	UFUNCTION(BlueprintCallable)
	UItemData* GetSlot(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	bool SetSlot(UItemData* Item, int32 Index, bool AllowOverwrite = false);

	UFUNCTION(BlueprintCallable)
	bool ContainsItem(UItemData* Item) const;

	UFUNCTION(BlueprintCallable)
	UItemData* PullFromSlot(int32 FromSlot);

	UFUNCTION(BlueprintCallable)
	UItemData* PullFromSlot_Internal(int32 FromSlot);

	UFUNCTION(BlueprintCallable)
	bool PushToSlot(UItemData* Item, int32 ToSlot, bool ReplaceContents = false);

	UFUNCTION(BlueprintCallable)
	bool PushToSlot_Internal(UItemData* Item, int32 ToSlot, bool ReplaceContents = false);

	UFUNCTION(BlueprintCallable)
	bool AddAmountOfTypeToSlot(int32 ToSlot, TSubclassOf<UItemAsset_Generic> Type, int32& Amount);

	UFUNCTION(BlueprintCallable)
	bool AddAmountOfTypeToSlot_Internal(int32 ToSlot, TSubclassOf<UItemAsset_Generic> Type, int32& Amount);

	UFUNCTION(BlueprintCallable)
	bool RemoveAmountOfTypeFromSlot(int32 FromSlot, TSubclassOf<UItemAsset_Generic> Type, int32 Amount);

	UFUNCTION(BlueprintCallable)
	bool RemoveAmountOfTypeFromSlot_Internal(int32 FromSlot, TSubclassOf<UItemAsset_Generic> Type, int32 Amount);

	UFUNCTION(BlueprintCallable)
	bool AddItemToInventory(UItemData* Item);

	UFUNCTION(BlueprintCallable)
	bool RemoveItemFromInventory(UItemData* Item);

	UFUNCTION(BlueprintCallable)
	bool RemoveAmountOfItemFromInventory(UItemData* Item, int32 Amount, int32& Remaining, bool AllowRemaining = true);

	UFUNCTION(BlueprintCallable)
	bool AddItemToInventory_Internal(UItemData* Item);

	UFUNCTION(BlueprintCallable)
	int32 CountAmountOfType(TSubclassOf<UItemAsset_Generic> Type);

	UFUNCTION(BlueprintCallable)
	bool AddAmountOfTypeToInventory(TSubclassOf<UItemAsset_Generic> Type, UPARAM(ref) int32& Amount);

	UFUNCTION(BlueprintCallable)
	bool AddAmountOfTypeToInventory_Internal(TSubclassOf<UItemAsset_Generic> Type, int32& Amount);

	UFUNCTION(BlueprintCallable)
	bool RemoveAmountOfTypeFromInventory(TSubclassOf<UItemAsset_Generic> Type, UPARAM(ref) int32& Amount);

	UFUNCTION(BlueprintCallable)
	bool RemoveAmountOfTypeFromInventory_Internal(TSubclassOf<UItemAsset_Generic> Type, int32& Amount);

	UFUNCTION(BlueprintCallable)
	bool SwapSlots(UInventoryData* FromInventory, int32 FromSlot, int32 ToSlot);

	UFUNCTION(BlueprintCallable)
	bool SwapSlots_Internal(UInventoryData* FromInventory, int32 FromSlot, int32 ToSlot);

	UFUNCTION(BlueprintCallable)
	bool TransferItemFromSlotToSlot(UInventoryData* FromInventory, int32 FromSlot, int32 ToSlot);

	UFUNCTION(BlueprintCallable)
	bool TransferItemFromSlotToSlot_Internal(UInventoryData* FromInventory, int32 FromSlot, int32 ToSlot);

	UFUNCTION(BlueprintCallable)
	bool TransferItemFromSlotToAnywhere(UInventoryData* FromInventory, int32 FromSlot);

	UFUNCTION(BlueprintCallable)
	bool TransferItemFromSlotToAnywhere_Internal(UInventoryData* FromInventory, int32 FromSlot);

	UPROPERTY(BlueprintAssignable, Category = "Dispatchers")
	FOnInventoryUpdated OnInventoryUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Dispatchers")
	FOnInventorySlotUpdated OnInventorySlotUpdated;
};
