// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/InventoryComponent.h"
#include "InventorySystem/InventoryItem.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	InventorySize = 8;

	ItemSlots.SetNum(InventorySize);
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::ResizeInventory()
{
	
}

void UInventoryComponent::AddToInventory_Internal(UInventoryItem* ItemToAdd)
{
	if(!IsValid(ItemToAdd)) return;
	bool CanMerge = ItemToAdd->CanMerge(nullptr);
	if (!CanMerge) {
		for (int32 i = 0; i < ItemSlots.Num(); i++) {
			if (ItemSlots[i] == nullptr || ItemSlots[i]->GetItemType() == ItemToAdd->GetItemType()){
				AddToSlot_Internal(i, ItemToAdd);
				break;
			}
		}
	}
	else {
		
	}
}

void UInventoryComponent::AddToSlot_Internal(int32 SlotNumber, UInventoryItem* ItemToAdd)
{
	if (ItemSlots[SlotNumber] == nullptr) {
		ItemSlots[SlotNumber] = ItemToAdd;
		ItemToAdd->Rename(nullptr, this);
	}
}

