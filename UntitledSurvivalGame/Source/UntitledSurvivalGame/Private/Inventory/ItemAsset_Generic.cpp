// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ItemAsset_Generic.h"

UItemAsset_Generic::UItemAsset_Generic()
{
	DisplayName	= FText::FromString("UNKNOWN");
	CanMerge	= true;
	MaxAmount	= 1;
	IsUsable	= false;
}

bool UItemAsset_Generic::OnUse_Implementation(APawn* InstigatingPlayerPawn)
{
    return false;
}
