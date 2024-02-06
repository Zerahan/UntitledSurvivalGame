// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/InventoryItem.h"
#include "InventorySystem/ItemDataAsset.h"
#include "Kismet/GameplayStatics.h"

UInventoryItem::UInventoryItem()
	: Super()
{
}

UTexture2D* UInventoryItem::GetIcon() const
{
	if(!ItemType->IsItemValid()) return UGameplayStatics::GetGameInstance(GetWorld())->GetEngine()->DefaultTexture;
	return ItemType->GetIcon();
}

UItemDataAsset* UInventoryItem::GetItemType() const
{
	return ItemType;
}

bool UInventoryItem::CanMerge_Implementation(UInventoryItem* ItemToMerge) const
{
	return false;
}

void UInventoryItem::DoMerge_Implementation(UInventoryItem* ItemToMerge)
{
	return;
}
