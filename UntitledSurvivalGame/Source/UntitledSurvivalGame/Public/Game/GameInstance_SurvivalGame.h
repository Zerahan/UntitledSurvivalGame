// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstance_SurvivalGame.generated.h"

class USaveGame_SurvivalGame;
class USaveGame_Settings;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UNTITLEDSURVIVALGAME_API UGameInstance_SurvivalGame : public UGameInstance
{
	GENERATED_BODY()
	/*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	FName PartialSlotName;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	FName FullSlotName;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	USaveGame_SurvivalGame* SaveGameData;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	USaveGame_Settings* SettingsData;

public:
	UGameInstance_SurvivalGame();
	
	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	const FString GetCurrentUserName() const;

	UFUNCTION(BlueprintCallable)
	bool LoadGame();

	UFUNCTION(BlueprintCallable)
	bool LoadGameFromSlot(const FString& Slot);

	UFUNCTION(BlueprintCallable)
	bool SaveGame();

	UFUNCTION(BlueprintCallable)
	bool SaveGameToSlot(const FString& Slot);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnFinishedLoading();
	virtual void OnFinishedLoading_Implementation();

	UFUNCTION(BlueprintCallable)
	const FString GetLastSavedSlot() const;
	//*/
};
