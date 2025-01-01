// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SaveGame_Settings.h"
#include "Kismet/GameplayStatics.h"

USaveGame_Settings::USaveGame_Settings()
{
	LastSavedSlotID = INDEX_NONE;
}

int32 USaveGame_Settings::GetLastSavedSlotID() const
{
    return LastSavedSlotID;
}

void USaveGame_Settings::SetLastSavedSlotID(int32 SlotID)
{
	LastSavedSlotID = SlotID;
}

bool USaveGame_Settings::WriteSaveDataToSlot(const FString& Slot)
{
	return UGameplayStatics::SaveGameToSlot(this, Slot, 0);
}
