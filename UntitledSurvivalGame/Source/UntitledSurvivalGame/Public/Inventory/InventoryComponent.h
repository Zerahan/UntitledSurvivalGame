// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UMenu_Inventory_Widget;
class UInventoryData;
class UItemData;
class UItemAsset_Generic;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryResized);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryLinked, UInventoryComponent*, LinkedInventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryDataSlotUpdated, int32, InventoryIndex, int32, SlotIndex);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTITLEDSURVIVALGAME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* LinkedInventoryRef;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UMenu_Inventory_Widget* InventoryWidget;

	UPROPERTY(SaveGame)
	TArray<UInventoryData*> InventoryDataList;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<int32> InventorySizes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FString InventoryName;
	
public:
	// Sets default values for this component's properties
	UInventoryComponent();

	UFUNCTION(BlueprintCallable)
	void SetInventoryName(FString NewName);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	UInventoryData* GetInventoryData(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	TArray<UInventoryData*> GetAllInventoryData() const;

	UFUNCTION(BlueprintCallable)
	UInventoryComponent* GetLinkedInventory() const;

	//UFUNCTION(BlueprintCallable)
	//void ResizeInventory(int32 NewSize, bool Destructive = false);

	UFUNCTION(BlueprintCallable)
	void SetLinkedInventory(UInventoryComponent* LinkedInventory);

	UFUNCTION(BlueprintCallable)
	void OpenInventoryWidget() const;

	UFUNCTION(BlueprintCallable)
	void OnInventorySlotUpdated(int32 InventoryIndex, int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	bool AddItemToInventory(UItemData* Item);

	UFUNCTION(BlueprintCallable)
	bool HasAmountOfItem(TSubclassOf<UItemAsset_Generic> Type, int32 Amount) const;

	UFUNCTION(BlueprintCallable)
	bool RemoveItemFromInventory(UItemData* Item);

	UFUNCTION(BlueprintCallable)
	bool RemoveAmountOfTypeFromInventory(TSubclassOf<UItemAsset_Generic> Type, int32 Amount);

	//UPROPERTY(BlueprintAssignable, Category = "Dispatchers")
	//FOnInventoryResized OnInventoryResized;

	UPROPERTY(BlueprintAssignable, Category = "Dispatchers")
	FOnInventoryLinked OnInventoryLinked;

	UPROPERTY(BlueprintAssignable, Category = "Dispatchers")
	FOnInventoryDataSlotUpdated OnInventoryDataSlotUpdated;
};
