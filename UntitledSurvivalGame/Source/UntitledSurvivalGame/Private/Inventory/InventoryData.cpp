// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryData.h"
#include "Inventory/ItemData.h"
#include "Inventory/ItemAsset_Generic.h"
#include "Inventory/ItemData.h"
#include "Inventory/ItemData_Quantity.h"

UInventoryData::UInventoryData()
	: Super()
{
	DataName = "DataName";
}

void UInventoryData::Initialize(int32 Index, int32 Size)
{
	DataIndex = Index;
	InventorySize = Size;
	Slots.Empty();
	Slots.SetNum(InventorySize);
}

void UInventoryData::SetDataName(FString NewName)
{
	DataName = NewName;
}

int32 UInventoryData::GetDataIndex() const
{
	return DataIndex;
}

UItemData* UInventoryData::GetSlot(int32 Index) const
{
	if(!Slots.IsValidIndex(Index)) return nullptr;
	return Slots[Index];
}

bool UInventoryData::SetSlot(UItemData* Item, int32 Index, bool AllowOverwrite)
{
	if (Slots.IsValidIndex(Index)) return false;
	if (!AllowOverwrite && IsValid(Slots[Index])) return false;
	if (IsValid(Item)) Item->Rename(nullptr, this);
	Slots[Index] = Item;
	return true;
}

bool UInventoryData::ContainsItem(UItemData* Item) const
{
	int32 Count = 0;
	for (int32 i = 0; i < Slots.Num(); i++) {
		if (Slots[i]->ClassRef != Item->ClassRef) continue;
		Count = Count + Slots[i]->GetAmount();
		if (Count >= Item->GetAmount()) return true;
	}
    return false;
}

UItemData* UInventoryData::PullFromSlot(int32 FromSlot)
{
	if (!Slots.IsValidIndex(FromSlot)) return nullptr;
	if (!IsValid(Slots[FromSlot])) return nullptr;
	return PullFromSlot_Internal(FromSlot);
}

UItemData* UInventoryData::PullFromSlot_Internal(int32 FromSlot)
{
	UItemData* Item = Slots[FromSlot];
	Slots[FromSlot] = nullptr;
	Item->Rename(nullptr, nullptr);
	return Item;
}

bool UInventoryData::PushToSlot(UItemData* Item, int32 ToSlot, bool ReplaceContents)
{
	if (!Slots.IsValidIndex(ToSlot)) return false;
	if (IsValid(Slots[ToSlot]) && !Item->GetStaticData()->CanMerge) return false;
	return PushToSlot_Internal(Item, ToSlot, ReplaceContents);
}

bool UInventoryData::PushToSlot_Internal(UItemData* Item, int32 ToSlot, bool ReplaceContents)
{
	if (ReplaceContents) {
		// Overwrite contents
		if (IsValid(Slots[ToSlot])) Slots[ToSlot]->MarkAsGarbage();
		Slots[ToSlot] = Item;
		return true;
	}
	else {
		// Add to empty slot
		if (!IsValid(Slots[ToSlot])) {
			Item->Rename(nullptr, this);
			Slots[ToSlot] = Item;
			return true;
		}
		else {
			//if(!Item->GetStaticData()->CanMerge) return false;
			Slots[ToSlot]->MergeData(Item);
			return true;
		}
	}
	//return false;
}

bool UInventoryData::AddAmountOfTypeToSlot(int32 ToSlot, TSubclassOf<UItemAsset_Generic> Type, int32& Amount)
{
	if (!Slots.IsValidIndex(ToSlot)) return false;
	if (Amount <= 0) return false;
	if (IsValid(Slots[ToSlot]) && Slots[ToSlot]->ClassRef != Type) return false;
	return AddAmountOfTypeToSlot_Internal(ToSlot, Type, Amount);
}

bool UInventoryData::AddAmountOfTypeToSlot_Internal(int32 ToSlot, TSubclassOf<UItemAsset_Generic> Type, int32& Amount)
{
	UItemData_Quantity* Item;
	if (!IsValid(Slots[ToSlot])) {
		Item = NewObject<UItemData_Quantity>(this);
		Item->ClassRef = Type;
		int32 Max = Item->GetStaticData()->MaxAmount;
		if (Amount <= Max) {
			Item->Amount = Amount;
			Amount = 0;
		}
		else {
			Item->Amount = Max;
			Amount = Amount - Max;
		}
		return PushToSlot_Internal(Item, ToSlot);
	}
	else {
		Item = Cast<UItemData_Quantity>(Slots[ToSlot]);
		int32 Max = Item->GetStaticData()->MaxAmount;
		if (Item->Amount == Max) return false;
		if (Max > Item->Amount + Amount) {
			Item->Amount = Item->Amount + Amount;
			Amount = 0;
		}
		else {
			Amount = Amount - (Max + Item->Amount);
			Item->Amount = Max;
		}
		Slots[ToSlot] = Item;
		return true;
	}
	//return false;
}

bool UInventoryData::RemoveAmountOfTypeFromSlot(int32 FromSlot, TSubclassOf<UItemAsset_Generic> Type, int32 Amount)
{
	if (!Slots.IsValidIndex(FromSlot)) return false;
	if (Amount <= 0) return false;
	if (!IsValid(Slots[FromSlot])) return false;
	if (Slots[FromSlot]->ClassRef != Type) return false;
	return RemoveAmountOfTypeFromSlot_Internal(FromSlot, Type, Amount);
}

bool UInventoryData::RemoveAmountOfTypeFromSlot_Internal(int32 FromSlot, TSubclassOf<UItemAsset_Generic> Type, int32 Amount)
{
	UItemData_Quantity* Item;
	if (!IsValid(Slots[FromSlot])) {
		Item = NewObject<UItemData_Quantity>(this);
		Item->ClassRef = Type;
		Item->Amount = Amount;
		Amount = 0;
		return PushToSlot_Internal(Item, FromSlot);
	}
	else {
		Item = Cast<UItemData_Quantity>(Slots[FromSlot]);
		if (Amount >= Item->Amount) {
			Amount = Amount - Item->Amount;
			Item->Amount = 0;
			Slots[FromSlot]->MarkAsGarbage();
			Slots[FromSlot] = nullptr;
		}
		else {
			Item->Amount = Item->Amount - Amount;
			Amount = 0;
			Slots[FromSlot] = Item;
		}
		return true;
	}
}

bool UInventoryData::AddItemToInventory(UItemData* Item)
{
	if (!IsValid(Item)) return false;
	if (!Item->IsItemValid()) return false;
	return AddItemToInventory_Internal(Item);
}

bool UInventoryData::RemoveItemFromInventory(UItemData* Item)
{
	int32 Index = -1;
	Slots.Find(Item, Index);
	if (Index == -1) return false;
	if (!PullFromSlot(Index)) return false;
	OnInventorySlotUpdated.Broadcast(DataIndex, Index);
	return true;
}

bool UInventoryData::RemoveAmountOfItemFromInventory(UItemData* Item, int32 Amount, int32& Remaining, bool AllowRemaining)
{
	int32 Index = -1;
	Slots.Find(Item, Index);
	if (Index == -1) return false;
	UItemData_Quantity* ItemData = Cast<UItemData_Quantity>(Item);
	if (!AllowRemaining && ItemData->GetAmount() < Amount) return false;
	ItemData->RemoveAmount(Amount);
	Remaining = Amount;
	if(ItemData->GetAmount() == 0){
		RemoveItemFromInventory(Item);
		Item->MarkAsGarbage();
		return true;
	}
	OnInventorySlotUpdated.Broadcast(DataIndex, Index);
	return true;
}

bool UInventoryData::AddItemToInventory_Internal(UItemData* Item)
{
	bool HasEmptySlots = false;
	if (Item->GetStaticData()->CanMerge) { // There's probably a better way to skip merging when an item can't be merged.
		for (int32 i = 0; i < Slots.Num(); i++) {
			if (!IsValid(Slots[i])) { HasEmptySlots = true; continue; }
			if (Slots[i]->ClassRef != Item->ClassRef) continue;
			Slots[i]->MergeData(Item);
			OnInventorySlotUpdated.Broadcast(DataIndex,i);
			if (!Item->IsItemValid()) return true;
		}
		// This can only get here when the item in question is still valid.
		if (!HasEmptySlots) return false;
	}
	// This function assumes the item's contained amount is less than or equal to its maximum. If it's possible to go over that max, this is where the seperator would exist.
	for (int32 i = 0; i < Slots.Num(); i++) {
		if (IsValid(Slots[i])) continue;
		if (PushToSlot_Internal(Item, i)) {
			OnInventorySlotUpdated.Broadcast(DataIndex,i);
			return true;
		}
		return false;
	}
	// The function *should* never get here. /shrug
	return false;
}

int32 UInventoryData::CountAmountOfType(TSubclassOf<UItemAsset_Generic> Type)
{
	int32 Count = 0;
	for (int32 i = 0; i < Slots.Num(); i++) {
		if (!IsValid(Slots[i])) continue;
		if (Slots[i]->ClassRef != Type) continue;
		Count = Count + Slots[i]->GetAmount();
	}
	return Count;
}

bool UInventoryData::AddAmountOfTypeToInventory(TSubclassOf<UItemAsset_Generic> Type, UPARAM(ref)int32& Amount)
{
	if (Amount <= 0) return false;
	if (!Type.GetDefaultObject()) return false;
	return AddAmountOfTypeToInventory_Internal(Type, Amount);
}

bool UInventoryData::AddAmountOfTypeToInventory_Internal(TSubclassOf<UItemAsset_Generic> Type, int32& Amount)
{
	if (Type.GetDefaultObject()->CanMerge) { // There's probably a better way to skip merging when an item can't be merged.
		int32 Max = Type.GetDefaultObject()->MaxAmount;
		for (int i = 0; i < Slots.Num(); i++) {
			if (!IsValid(Slots[i])) continue;
			if (Slots[i]->ClassRef != Type) continue;
			if (Slots[i]->GetAmount() >= Max) continue;
			AddAmountOfTypeToSlot_Internal(i, Type, Amount);
			OnInventorySlotUpdated.Broadcast(DataIndex,i);
			if (Amount <= 0) return true;
		}
	}
	for (int i = 0; i < Slots.Num(); i++) {
		if (IsValid(Slots[i])) continue;
		AddAmountOfTypeToSlot_Internal(i, Type, Amount);
		OnInventorySlotUpdated.Broadcast(DataIndex,i);
		if (Amount <= 0) return true;
	}
	return false;
}

bool UInventoryData::RemoveAmountOfTypeFromInventory(TSubclassOf<UItemAsset_Generic> Type, UPARAM(ref)int32& Amount)
{
	if (Amount <= 0) return false;
	if (CountAmountOfType(Type) <= 0) return false;
	return RemoveAmountOfTypeFromInventory_Internal(Type, Amount);
}

bool UInventoryData::RemoveAmountOfTypeFromInventory_Internal(TSubclassOf<UItemAsset_Generic> Type, int32& Amount)
{
	for (int32 i = 0; i < Slots.Num(); i++) {
		if (!IsValid(Slots[i])) continue;
		if (Slots[i]->ClassRef != Type) continue;
		if (Amount >= Slots[i]->GetAmount()) {
			Amount = Amount - Slots[i]->GetAmount();
			Slots[i]->MarkAsGarbage();
			Slots[i] = nullptr;
			OnInventorySlotUpdated.Broadcast(DataIndex,i);
		}
		else {
			// Theoretically, this can *only* run on inventory slots that have an amount greater than 1, which guaruntees they are itemdata with quantity?
			if (!Slots[i]->GetClass()->IsChildOf(UItemData_Quantity::StaticClass())) {
				UE_LOG(LogTemp, Warning, TEXT("ERROR: InventoryComponent::RemoveAmountOfTypeFromInventory_Internal(): This should literally never happen. But just in case..."));
				continue;
			}
			UItemData_Quantity* Item = Cast<UItemData_Quantity>(Slots[i]);
			Item->Amount = Item->Amount - Amount;
			Amount = 0;
			Slots[i] = Item;
			OnInventorySlotUpdated.Broadcast(DataIndex,i);
			return true;
		}
		if (Amount <= 0) return true;
	}
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("failed horrifically?")));
	return false;
}

bool UInventoryData::SwapSlots(UInventoryData* FromInventory, int32 FromSlot, int32 ToSlot)
{
	if (!IsValid(FromInventory)) return false;									// No point trying if the source inventory is invalid...
	if (!Slots.IsValidIndex(ToSlot)) return false;						// ...if the slot we're transferring to is out of range...
	if (!FromInventory->Slots.IsValidIndex(FromSlot)) return false;	// ...if the slot we're coming from is out of range...
	if (!IsValid(FromInventory->Slots[FromSlot])) return false;		// ...if we're trying to transfer nothing...
	if (FromInventory == this && FromSlot == ToSlot) return false;
	//if (FromInventory->Slots[FromSlot]->IsItemValid()) return false;	// ...or if we're trying to transfer an invalid item. (Should probably fix the invalid data here).
	return SwapSlots_Internal(FromInventory, FromSlot, ToSlot);
}

bool UInventoryData::SwapSlots_Internal(UInventoryData* FromInventory, int32 FromSlot, int32 ToSlot)
{
	UItemData* ToItem = PullFromSlot_Internal(ToSlot);
	UItemData* FromItem = FromInventory->PullFromSlot_Internal(FromSlot);
	PushToSlot_Internal(FromItem, ToSlot);
	FromInventory->PushToSlot_Internal(ToItem, FromSlot);

	OnInventorySlotUpdated.Broadcast(DataIndex,ToSlot);
	FromInventory->OnInventorySlotUpdated.Broadcast(FromInventory->GetDataIndex(),FromSlot);
	return true;
}

bool UInventoryData::TransferItemFromSlotToSlot(UInventoryData* FromInventory, int32 FromSlot, int32 ToSlot)
{
	if (!IsValid(FromInventory)) return false;
	if (!Slots.IsValidIndex(ToSlot)) return false;
	if (!FromInventory->Slots.IsValidIndex(FromSlot)) return false;
	if (!IsValid(FromInventory->Slots[FromSlot])) return false;
	//if (FromInventory->Slots[FromSlot]->IsItemValid()) return false;
	if (FromInventory == this && FromSlot == ToSlot) return false;
	return TransferItemFromSlotToSlot_Internal(FromInventory, FromSlot, ToSlot);
}

bool UInventoryData::TransferItemFromSlotToSlot_Internal(UInventoryData* FromInventory, int32 FromSlot, int32 ToSlot)
{
	if (!IsValid(Slots[ToSlot])) {
		PushToSlot_Internal(FromInventory->PullFromSlot_Internal(FromSlot), ToSlot, true);
	}
	else {
		if (Slots[ToSlot]->ClassRef == FromInventory->Slots[FromSlot]->ClassRef) {
			PushToSlot_Internal(FromInventory->Slots[FromSlot], ToSlot, false);
			if (FromInventory->Slots[FromSlot]->GetAmount() == 0) {
				FromInventory->Slots[FromSlot]->MarkAsGarbage();
				FromInventory->Slots[FromSlot] = nullptr;
			}
		}
		else {
			SwapSlots_Internal(FromInventory, FromSlot, ToSlot);
		}
	}
	OnInventorySlotUpdated.Broadcast(DataIndex,ToSlot);
	FromInventory->OnInventorySlotUpdated.Broadcast(FromInventory->GetDataIndex(),FromSlot);
	return true;
}

bool UInventoryData::TransferItemFromSlotToAnywhere(UInventoryData* FromInventory, int32 FromSlot)
{
	if (!IsValid(FromInventory)) return false;
	if (!FromInventory->Slots.IsValidIndex(FromSlot)) return false;
	if (!IsValid(FromInventory->Slots[FromSlot])) return false;
	return TransferItemFromSlotToAnywhere_Internal(FromInventory, FromSlot);
}

bool UInventoryData::TransferItemFromSlotToAnywhere_Internal(UInventoryData* FromInventory, int32 FromSlot)
{
	if (AddItemToInventory(FromInventory->GetSlot(FromSlot))) {
		if (!IsValid(FromInventory->Slots[FromSlot])) {
			FromInventory->OnInventorySlotUpdated.Broadcast(FromInventory->GetDataIndex(),FromSlot);
			return true;
		}
		if (!FromInventory->Slots[FromSlot]->IsItemValid()) {
			FromInventory->Slots[FromSlot]->MarkAsGarbage();
			FromInventory->Slots[FromSlot] = nullptr;
			FromInventory->OnInventorySlotUpdated.Broadcast(FromInventory->GetDataIndex(),FromSlot);
			return true;
		}
	}
	return false;
}

