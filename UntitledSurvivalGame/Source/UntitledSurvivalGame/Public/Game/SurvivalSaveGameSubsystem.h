// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SurvivalSaveGameSubsystem.generated.h"

class USaveGame_SurvivalGame;
class USaveGame_Settings;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReloadUserData);

/**
 * 
 */
UCLASS()
class UNTITLEDSURVIVALGAME_API USurvivalSaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	FName OwningUserName;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	USaveGame_SurvivalGame* SaveGameData;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	USaveGame_Settings* SettingsData;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	int32 SaveSlotID;
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UFUNCTION(BlueprintCallable)
	const FString GetCurrentUserName() const;

	UFUNCTION(BlueprintCallable)
	const FString GetFormatedSaveSlot(int32 SaveID = -1) const;

	UFUNCTION(BlueprintCallable)
	void CheckForUserChanged();
	
	UFUNCTION(BlueprintCallable)
	bool HasUserChanged() const;

	UFUNCTION(BlueprintCallable)
	void ReloadUserData();

	UFUNCTION(BlueprintCallable)
	void SaveSettings();

	UFUNCTION(BlueprintCallable)
	UWorld* GetWorldFromPlayer();

public:
	UFUNCTION(BlueprintCallable)
	bool HasValidLastGameID() const;

	UFUNCTION(BlueprintCallable)
	bool NewGame(int32 SlotID);

	UFUNCTION(BlueprintCallable)
	bool LoadGame();

	UFUNCTION(BlueprintCallable)
	bool LoadGameFromID(int32 SlotID);

	UFUNCTION()
	void OnLevelLoaded();

	UFUNCTION(BlueprintCallable)
	bool SaveGame();

	UFUNCTION(BlueprintCallable)
	bool SaveGameFromID(int32 SlotID);

	UFUNCTION(BlueprintCallable)
	bool DeleteSaveGame(int32 SlotID);
	
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnFinishedLoading();
	virtual void OnFinishedLoading_Implementation();


public:
	UPROPERTY(BlueprintAssignable)
	FOnReloadUserData OnReloadUserData;
};
