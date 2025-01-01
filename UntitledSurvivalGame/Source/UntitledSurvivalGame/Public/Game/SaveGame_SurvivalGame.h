// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGame_SurvivalGame.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishedLoading);

USTRUCT()
struct FGenericActorSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	TArray<uint8> ByteData;
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class UNTITLEDSURVIVALGAME_API USaveGame_SurvivalGame : public USaveGame
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	FString FullSlotName;

	UPROPERTY()
	TMap<FName, FGenericActorSaveData> SavedActorData;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	FName LevelName;
	
public:
	USaveGame_SurvivalGame();
	
	UFUNCTION(BlueprintCallable)
	FName GetLevelName() const;

	UFUNCTION(BlueprintCallable)
	bool WriteSaveGame(UWorld* World);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool WriteSaveGameToSlot(const FString& ToFullSlotName);
	virtual bool WriteSaveGameToSlot_Implementation(const FString& ToFullSlotName);

	UFUNCTION(BlueprintCallable)
	bool LoadSaveData(UWorld* World);

	UFUNCTION(BlueprintCallable)
	bool ReadSaveGame();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool ReadSaveGameFromSlot(const FString& ToFullSlotName);
	virtual bool ReadSaveGameFromSlot_Implementation(const FString& ToFullSlotName);

	UFUNCTION(BlueprintCallable)
	int32 GetCount() const;

	UPROPERTY(BlueprintAssignable)
	FOnFinishedLoading OnFinishedLoading;
};
