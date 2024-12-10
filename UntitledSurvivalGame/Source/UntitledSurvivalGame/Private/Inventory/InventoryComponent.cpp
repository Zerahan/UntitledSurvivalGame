// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryData.h"
#include "Inventory/ItemData.h"
#include "Inventory/Menu_Inventory_Widget.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	InventoryName = "GenericInventory";

	// ...
}

void UInventoryComponent::SetInventoryName(FString NewName)
{
	InventoryName = NewName;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if(InventorySizes.Num() == 0){
		UE_LOG(LogTemp, Warning, TEXT("WARNING: Inventory on (%s) initialized with no sizes specified. Using default."), *GetOutermostObject()->GetFName().ToString());
		InventorySizes.Add(4);
	}

	InventoryDataList.SetNum(InventorySizes.Num());

	for (int i = 0; i < InventorySizes.Num(); i++) {
		InventoryDataList[i] = NewObject<UInventoryData>(this);
		InventoryDataList[i]->Initialize(i, InventorySizes[i]);
		InventoryDataList[i]->OnInventorySlotUpdated.AddDynamic(this, &UInventoryComponent::OnInventorySlotUpdated);
	}
	
	//ResizeInventory(InventorySize, true);

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UInventoryData* UInventoryComponent::GetInventoryData(int32 Index) const
{
	if(!InventoryDataList.IsValidIndex(Index)) return nullptr;
	return InventoryDataList[Index];
}

TArray<UInventoryData*> UInventoryComponent::GetAllInventoryData() const
{
	return InventoryDataList;
}

UInventoryComponent* UInventoryComponent::GetLinkedInventory() const
{
    return LinkedInventoryRef;
}

/*
void UInventoryComponent::ResizeInventory(int32 NewSize, bool Destructive)
{
	if (Destructive) {
		InventorySlots.SetNum(InventorySize);
	}
	else {

	}
	OnInventoryResized.Broadcast();
}
//*/

void UInventoryComponent::SetLinkedInventory(UInventoryComponent* LinkedInventory)
{
	LinkedInventoryRef = LinkedInventory;
	OnInventoryLinked.Broadcast(LinkedInventory);
}

void UInventoryComponent::OpenInventoryWidget() const
{
	if (InventoryWidget) {
		InventoryWidget->GetOwningPlayer()->SetShowMouseCursor(true);
		InventoryWidget->GetOwningPlayer()->SetInputMode(FInputModeUIOnly::FInputModeUIOnly());
		InventoryWidget->AddToViewport();
	}
}

void UInventoryComponent::OnInventorySlotUpdated(int32 InventoryIndex, int32 SlotIndex)
{
	OnInventoryDataSlotUpdated.Broadcast(InventoryIndex, SlotIndex);
}

bool UInventoryComponent::AddItemToInventory(UItemData* Item)
{
	for (int32  i = 0; i < InventoryDataList.Num(); i++) {
		if(InventoryDataList[i]->AddItemToInventory(Item)) return true;
	}
	return false;
}

bool UInventoryComponent::HasAmountOfItem(TSubclassOf<UItemAsset_Generic> Type, int32 Amount) const
{
	int32 Total = 0;
	for (int32 i = 0; i < InventoryDataList.Num(); i++) {
		Total = Total + InventoryDataList[i]->CountAmountOfType(Type);
		if(Total >= Amount) return true;
	}
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Found %d of item"), Total));
	return false;
}

bool UInventoryComponent::RemoveItemFromInventory(UItemData* Item)
{
	for (int32 i = 0; i < InventoryDataList.Num(); i++) {
		if(InventoryDataList[i]->RemoveItemFromInventory(Item)) return true;
	}
	return false;
}

bool UInventoryComponent::RemoveAmountOfTypeFromInventory(TSubclassOf<UItemAsset_Generic> Type, int32 Amount)
{
	int32 Remaining = Amount;
	for (int32 i = 0; i < InventoryDataList.Num(); i++) {
		InventoryDataList[i]->RemoveAmountOfTypeFromInventory(Type, Remaining);
		if(Remaining <= 0) return true;
	}
    return false;
}

