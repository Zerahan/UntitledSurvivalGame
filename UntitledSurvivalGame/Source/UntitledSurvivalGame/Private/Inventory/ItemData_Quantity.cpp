// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ItemData_Quantity.h"
#include "Inventory/ItemAsset_Generic.h"

int32 UItemData_Quantity::GetAmount_Implementation() const
{
    return Amount;
}

bool UItemData_Quantity::IsItemValid_Implementation() const
{
    return Super::IsItemValid_Implementation() && (GetStaticData()->CanMerge ? Amount > 0 : Amount >= 0);
}

bool UItemData_Quantity::CanMerge_Implementation(UItemData* DataToMerge) const
{
	if(!IsValid(DataToMerge)) return false;
	if(DataToMerge->ClassRef != ClassRef) return false;
	if(!GetStaticData()->CanMerge) return false;
	return Amount < GetStaticData()->MaxAmount;
}

bool UItemData_Quantity::MergeData_Internal_Implementation(UItemData* DataToMerge)
{
	UItemData_Quantity* OtherData = Cast<UItemData_Quantity>(DataToMerge);
	int32 MaxAmount		= GetStaticData()->MaxAmount;
	int32 MyAmount		= Amount;
	int32 OtherAmount	= OtherData->Amount;
	int32 Capacity		= MaxAmount - Amount;
	if (OtherAmount <= Capacity) {
		MyAmount	= MyAmount + OtherAmount;
		OtherAmount = 0;
	}
	else {
		MyAmount	= MaxAmount;
		OtherAmount = OtherAmount - Capacity;
	}
	Amount				= MyAmount;
	OtherData->Amount	= OtherAmount;
	DataToMerge			= OtherData;
	return true;
}

bool UItemData_Quantity::AddAmount_Implementation(int32& Value)
{
	int32 Rem = GetStaticData()->MaxAmount - Amount;
	if (Rem == 0) return false;
	if (Value <= Rem) {
		Amount	= Amount + Value;
		Value	= 0;
	}
	else {
		Amount	= GetStaticData()->MaxAmount;
		Value	= Value - Rem;
	}
	return true;
}

bool UItemData_Quantity::RemoveAmount_Implementation(int32& Value)
{
	if (Amount == 0) return false;
	if (Amount >= Value) {
		Amount	= Amount - Value;
		Value	= 0;
	}else{
		Value	= Amount;
		Amount	= 0;
	}

	return true;
}
