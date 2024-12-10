// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Menu_Inventory_Widget.h"
#include "Inventory/InventoryComponent.h"

void UMenu_Inventory_Widget::SetInventoryRef(UInventoryComponent* NewInventoryRef)
{
	//if (InventoryRef) InventoryRef->OnInventoryUpdated.RemoveDynamic(this, &UMenu_Inventory_Widget::OnInventoryUpdated);
	if (InventoryRef) InventoryRef->OnInventoryLinked.RemoveDynamic(this, &UMenu_Inventory_Widget::OnLinkedInventoryUpdated);
	if (InventoryRef) InventoryRef->OnInventoryDataSlotUpdated.RemoveDynamic(this, &UMenu_Inventory_Widget::OnInventorySlotUpdated);
	InventoryRef = NewInventoryRef;
	//InventoryRef->OnInventoryUpdated.AddDynamic(this, &UMenu_Inventory_Widget::OnInventoryUpdated);
	InventoryRef->OnInventoryLinked.AddDynamic(this, &UMenu_Inventory_Widget::OnLinkedInventoryUpdated);
	InventoryRef->OnInventoryDataSlotUpdated.AddDynamic(this, &UMenu_Inventory_Widget::OnInventorySlotUpdated);
	//SetInventorySize(InventoryRef->InventorySize);
}

void UMenu_Inventory_Widget::OnInventoryUpdated_Implementation()
{
}

void UMenu_Inventory_Widget::OnInventorySlotUpdated_Implementation(int32 InventoryIndex, int32 SlotIndex)
{
}

void UMenu_Inventory_Widget::OnLinkedInventoryUpdated_Implementation(UInventoryComponent* NewLinkedInventoryRef)
{
}

void UMenu_Inventory_Widget::SetInventorySize_Implementation(int32 NewSize)
{
	InventorySize = NewSize;
}
