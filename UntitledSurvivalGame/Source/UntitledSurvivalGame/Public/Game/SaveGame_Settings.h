// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGame_Settings.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UNTITLEDSURVIVALGAME_API USaveGame_Settings : public USaveGame
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	int32 LastSavedSlotID;

public:
	USaveGame_Settings();

	UFUNCTION(BlueprintCallable)
	int32 GetLastSavedSlotID() const;
	
	UFUNCTION()
	void SetLastSavedSlotID(int32 SlotID);

	UFUNCTION(BlueprintCallable)
	bool WriteSaveDataToSlot(const FString& Slot);
};
