// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/ItemDataAsset.h"
const FText UItemDataAsset::DEFAULT_NAME(FText::FromString("DEFAULT_NAME"));

UItemDataAsset::UItemDataAsset()
	: Super()
{
	DisplayName = DEFAULT_NAME;
}

UTexture2D* UItemDataAsset::GetIcon() const
{
	return Icon.LoadSynchronous();
}

bool UItemDataAsset::IsItemValid_Implementation() const
{
	return !DisplayName.EqualTo(DEFAULT_NAME);
}
