// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameInstance_SurvivalGame.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Game/SaveGame_SurvivalGame.h"
#include "Game/SaveGame_Settings.h"
#include "Game/Power/PowerNetworkNode.h"
#include "Game/Power/PowerSystemData.h"
/*/
UGameInstance_SurvivalGame::UGameInstance_SurvivalGame()
{
}

void UGameInstance_SurvivalGame::Init()
{
	Super::Init();
	if (UGameplayStatics::DoesSaveGameExist(GetCurrentUserName() + "_Settings", 0)) {
		SettingsData = CastChecked<USaveGame_Settings>(UGameplayStatics::LoadGameFromSlot(GetCurrentUserName() + "_Settings", 0));
	}
	else {
		SettingsData = CastChecked<USaveGame_Settings>(UGameplayStatics::CreateSaveGameObject(USaveGame_Settings::StaticClass()));
	}
}

const FString UGameInstance_SurvivalGame::GetCurrentUserName() const
{
	return UKismetSystemLibrary::GetPlatformUserName();
}

bool UGameInstance_SurvivalGame::LoadGame()
{
	return LoadGameFromSlot(PartialSlotName.ToString());
}

bool UGameInstance_SurvivalGame::LoadGameFromSlot(const FString& Slot)
{
	if (Slot == "") return false;
	PartialSlotName = FName(Slot);
	FullSlotName = FName(GetCurrentUserName() + "_" + Slot);
	if (!UGameplayStatics::DoesSaveGameExist(FullSlotName.ToString(), 0)) {
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("New save data created.")));
		SaveGameData = CastChecked<USaveGame_SurvivalGame>(UGameplayStatics::CreateSaveGameObject(USaveGame_SurvivalGame::StaticClass()));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, FString::Printf(TEXT("Load save data.")));
		SaveGameData = CastChecked<USaveGame_SurvivalGame>(UGameplayStatics::LoadGameFromSlot(FullSlotName.ToString(), 0));
		GEngine->AddOnScreenDebugMessage(-1,10,FColor::Red, FString::Printf(TEXT("FoundCount: %s"), *FString::FromInt(SaveGameData->GetCount())));
	}
	if(!IsValid(SaveGameData)) return false;
	SaveGameData->Rename(nullptr, this);
	//SaveGameData->WriteSaveGameToSlot(FullSlotName.ToString());
	
	if (IsValid(SettingsData)) {
		SettingsData->SetLastSavedSlot(PartialSlotName.ToString());
		SettingsData->WriteSaveDataToSlot(GetCurrentUserName() + "_Settings");
	}
	SaveGameData->OnFinishedLoading.AddDynamic(this, &UGameInstance_SurvivalGame::OnFinishedLoading);
	
	APlayerController* Player = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), 0);
	if (IsValid(Player)) {
		Player->SetInputMode(FInputModeGameOnly());
		Player->SetShowMouseCursor(false);
	}

	UGameplayStatics::OpenLevel(GetWorld(), SaveGameData->GetLevelName(), true, FString("Game=SurvivalGM_Base_BP"));
	SaveGameData->ReadSaveGameFromSlot(FullSlotName.ToString());

	return true;
}

bool UGameInstance_SurvivalGame::SaveGame()
{
	return SaveGameToSlot(PartialSlotName.ToString());
}

bool UGameInstance_SurvivalGame::SaveGameToSlot(const FString& Slot)
{
	if (Slot == "") return false;
	PartialSlotName = FName(Slot);
	FullSlotName = FName(GetCurrentUserName() + "_" + Slot);
	SaveGameData->WriteSaveGameToSlot(FullSlotName.ToString());
	return true;
}

void UGameInstance_SurvivalGame::OnFinishedLoading_Implementation()
{
	TArray<AActor*> Nodes;
	TArray<UPowerSystemData*> PowerSystems;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APowerNetworkNode::StaticClass(), Nodes);
	for (AActor* Node : Nodes) {
		APowerNetworkNode* Ref = Cast<APowerNetworkNode>(Node);
		int32 Index = PowerSystems.AddUnique(Ref->GetLinkedPowerSystem());
		if(Index != INDEX_NONE) continue;
		Ref->GetLinkedPowerSystem()->Recalculate();
	}
}

const FString UGameInstance_SurvivalGame::GetLastSavedSlot() const
{
	//FString Slot;
	//SettingsData->GetLastSavedSlot(Slot);
	if(!IsValid(SettingsData)) return FString();
	return FString(SettingsData->GetLastSavedSlot().ToString());
}

//*/