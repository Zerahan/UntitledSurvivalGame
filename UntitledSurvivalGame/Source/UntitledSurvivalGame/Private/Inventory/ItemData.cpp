// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ItemData.h"
#include "Inventory/ItemAsset_Generic.h"
#include "Inventory/InventoryComponent.h"

int32 UItemData::GetAmount_Implementation() const
{
	return 1;
}

TSubclassOf<UItemAsset_Generic> UItemData::GetClassRef() const
{
	return ClassRef;
}

UItemAsset_Generic* UItemData::GetClassDefaultObject() const
{
	return Cast<UItemAsset_Generic>(ClassRef.GetDefaultObject());
}

UItemAsset_Generic* UItemData::GetStaticData() const
{
    return ClassRef.GetDefaultObject();
}

UInventoryComponent* UItemData::GetOwningInventory() const
{
	return Cast<UInventoryComponent>(GetOuter());
}

bool UItemData::IsItemValid_Implementation() const
{
	return ClassRef != nullptr;
}

bool UItemData::CanMerge_Implementation(UItemData* DataToMerge) const
{
	return false;
}

bool UItemData::MergeData(UItemData* DataToMerge)
{
	if(!IsValid(DataToMerge)) return false;
	if(!CanMerge(DataToMerge)) return false;
    return MergeData_Internal(DataToMerge);
}

bool UItemData::MergeData_Internal_Implementation(UItemData* DataToMerge)
{
	return false;
}
