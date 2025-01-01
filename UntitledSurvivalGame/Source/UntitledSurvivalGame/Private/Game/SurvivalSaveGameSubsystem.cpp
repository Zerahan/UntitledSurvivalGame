// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SurvivalSaveGameSubsystem.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Game/SaveGame_Settings.h"
#include "Game/SaveGame_SurvivalGame.h"
#include "Game/Power/PowerNetworkNode.h"
#include "Game/Power/PowerSystemData.h"

void USurvivalSaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Initialized savesystem!")));

	ReloadUserData();
}

const FString USurvivalSaveGameSubsystem::GetCurrentUserName() const
{
	return OwningUserName.ToString();
}

const FString USurvivalSaveGameSubsystem::GetFormatedSaveSlot(int32 SaveID) const
{
	if(SaveID == -1) return FString(GetCurrentUserName() + "_Slot" + FString::FromInt(SaveSlotID));
	return FString(GetCurrentUserName() + "_Slot" + FString::FromInt(SaveID));
}

void USurvivalSaveGameSubsystem::CheckForUserChanged()
{
	if(!HasUserChanged()) return;
	ReloadUserData();
}

bool USurvivalSaveGameSubsystem::HasUserChanged() const
{
	return OwningUserName != UKismetSystemLibrary::GetPlatformUserName();
}

void USurvivalSaveGameSubsystem::ReloadUserData()
{
	OwningUserName = FName(UKismetSystemLibrary::GetPlatformUserName());
	
	if (!UGameplayStatics::DoesSaveGameExist(GetCurrentUserName() + "_Settings", 0)) {
		SettingsData = CastChecked<USaveGame_Settings>(UGameplayStatics::CreateSaveGameObject(USaveGame_Settings::StaticClass()));
		SaveSettings();
	} else {
		SettingsData = CastChecked<USaveGame_Settings>(UGameplayStatics::LoadGameFromSlot(GetCurrentUserName() + "_Settings", 0));
	}

	OnReloadUserData.Broadcast();
}

void USurvivalSaveGameSubsystem::SaveSettings()
{
	UGameplayStatics::SaveGameToSlot(SettingsData, GetCurrentUserName() + "_Settings", 0);
}

UWorld* USurvivalSaveGameSubsystem::GetWorldFromPlayer()
{
	return UGameplayStatics::GetPlayerController(GetWorld(),0)->GetWorld();
}

bool USurvivalSaveGameSubsystem::HasValidLastGameID() const
{
	if(!IsValid(SettingsData)) return false;
	return SettingsData->GetLastSavedSlotID() != INDEX_NONE;
}

bool USurvivalSaveGameSubsystem::NewGame(int32 SlotID)
{
	if (SlotID == INDEX_NONE) return false;
	SaveGameData = CastChecked<USaveGame_SurvivalGame>(UGameplayStatics::CreateSaveGameObject(USaveGame_SurvivalGame::StaticClass()));
	UGameplayStatics::SaveGameToSlot(SaveGameData, GetFormatedSaveSlot(SlotID), 0);
	return LoadGameFromID(SlotID);
}

bool USurvivalSaveGameSubsystem::LoadGame()
{
	if(!IsValid(SettingsData)) return false;
	return LoadGameFromID(SettingsData->GetLastSavedSlotID());
}

bool USurvivalSaveGameSubsystem::LoadGameFromID(int32 SlotID)
{
	if (SlotID == -1) return false;
	FString SaveSlot = GetFormatedSaveSlot(SlotID);
	if (!UGameplayStatics::DoesSaveGameExist(SaveSlot, 0)) return false;
	SettingsData->SetLastSavedSlotID(SlotID);
	SaveSettings();

	SaveGameData = CastChecked<USaveGame_SurvivalGame>(UGameplayStatics::LoadGameFromSlot(GetFormatedSaveSlot(SlotID), 0));
	if(!IsValid(SaveGameData)) return false;
	
	SaveSlotID = SlotID;

	SaveGameData->OnFinishedLoading.AddDynamic(this, &USurvivalSaveGameSubsystem::OnFinishedLoading);

	
	APlayerController* Player = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), 0);
	if (IsValid(Player)) {
		Player->SetInputMode(FInputModeGameOnly());
		Player->SetShowMouseCursor(false);
	}

	
	//UGameplayStatics::OpenLevel(GetWorld(), FName("Map_Transition"), true);
	UGameplayStatics::OpenLevel(GetWorld(), SaveGameData->GetLevelName(), true, FString("Game=SurvivalGM_Base_BP"));
	UGameplayStatics::LoadStreamLevel(GetWorld(), SaveGameData->GetLevelName(), true, true, FLatentActionInfo(0, 123456, TEXT("OnLevelLoaded"), this));

	return true;
}

void USurvivalSaveGameSubsystem::OnLevelLoaded()
{
	SaveGameData->LoadSaveData(GetWorld());
	
	SaveGameData->OnFinishedLoading.RemoveDynamic(this, &USurvivalSaveGameSubsystem::OnFinishedLoading);
}

bool USurvivalSaveGameSubsystem::SaveGame()
{
	return SaveGameFromID(SaveSlotID);
}

bool USurvivalSaveGameSubsystem::SaveGameFromID(int32 SlotID)
{
	if (SlotID == INDEX_NONE) return false;
	if (SaveSlotID != SlotID) {
		SaveSlotID = SlotID;
		if (IsValid(SettingsData)) {
			SettingsData->SetLastSavedSlotID(SlotID);
			SaveSettings();
		}
	}
	SaveGameData->WriteSaveGame(GetWorld());
	UGameplayStatics::SaveGameToSlot(SaveGameData, GetFormatedSaveSlot(SlotID), 0);

	return false;
}

bool USurvivalSaveGameSubsystem::DeleteSaveGame(int32 SlotID)
{
	if(SlotID == INDEX_NONE) return false;
	if(IsValid(SettingsData)) if(SettingsData->GetLastSavedSlotID() == SlotID) SettingsData->SetLastSavedSlotID(-1);
	return UGameplayStatics::DeleteGameInSlot(GetFormatedSaveSlot(SlotID), 0);
}

void USurvivalSaveGameSubsystem::OnFinishedLoading_Implementation()
{
	TArray<AActor*> Nodes;
	TArray<UPowerSystemData*> PowerSystems;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APowerNetworkNode::StaticClass(), Nodes);
	for (AActor* Node : Nodes) {
		APowerNetworkNode* Ref = Cast<APowerNetworkNode>(Node);
		int32 Index = PowerSystems.AddUnique(IPowerSystemInterface::Execute_GetLinkedPowerSystem(Ref));
		if (Index == INDEX_NONE) continue;
		PowerSystems[Index]->OnFinishedLoad();
	}
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Magenta, FString::Printf(TEXT("Finished Loading!")));
}
