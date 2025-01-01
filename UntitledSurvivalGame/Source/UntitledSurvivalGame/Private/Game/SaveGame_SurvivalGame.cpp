// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SaveGame_SurvivalGame.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Interfaces/SaveSystemInterface.h"

USaveGame_SurvivalGame::USaveGame_SurvivalGame()
{
	LevelName = FName("devTest");
}

FName USaveGame_SurvivalGame::GetLevelName() const
{
	return LevelName;
}

bool USaveGame_SurvivalGame::WriteSaveGame(UWorld* World)
{
	SavedActorData.Empty();

	for (AActor* Actor : TActorRange<AActor>(World))
	{
		if (!IsValid(Actor)) continue;
		if (!Actor->GetClass()->ImplementsInterface(USaveSystemInterface::StaticClass())) continue;

		FGenericActorSaveData Data;
		Data.Transform = Actor->GetActorTransform();

		FMemoryWriter MWriter(Data.ByteData);
		FObjectAndNameAsStringProxyArchive ProxObj(MWriter, true);
		ProxObj.ArIsSaveGame = true;
		Actor->Serialize(ProxObj);
		SavedActorData.Add(Actor->GetFName(), Data);
	}

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, FString::Printf(TEXT("Save Complete")));
	return true;
}

bool USaveGame_SurvivalGame::WriteSaveGameToSlot_Implementation(const FString& ToFullSlotName)
{
	FullSlotName = ToFullSlotName;
	SavedActorData.Empty();

	for (AActor* Actor : TActorRange<AActor>(GetWorld()))
	{
		if(!IsValid(Actor)) continue;
		if(!Actor->GetClass()->ImplementsInterface(USaveSystemInterface::StaticClass())) continue;

		FGenericActorSaveData Data;
		Data.Transform = Actor->GetActorTransform();

		FMemoryWriter MWriter(Data.ByteData);
		FObjectAndNameAsStringProxyArchive ProxObj(MWriter, true);
		ProxObj.ArIsSaveGame = true;
		Actor->Serialize(ProxObj);
		SavedActorData.Add(Actor->GetFName(), Data);
	}

	GEngine->AddOnScreenDebugMessage(-1,10,FColor::Cyan, FString::Printf(TEXT("Save Complete")));
	return UGameplayStatics::SaveGameToSlot(this, FullSlotName, 0);
}

bool USaveGame_SurvivalGame::LoadSaveData(UWorld* World)
{
	int32 Counter = 0;
	int32 FoundCount = 0;
	
	for (TActorIterator<AActor> Itr(World); Itr; ++Itr) {
		AActor* Actor = *Itr;
		if (!IsValid(Actor)) continue;
		if (!Actor->GetClass()->ImplementsInterface(USaveSystemInterface::StaticClass())) continue;
		FoundCount++;

		FGenericActorSaveData* Data = SavedActorData.Find(Actor->GetFName());
		if (Data) {
			Actor->SetActorTransform(Data->Transform);

			FMemoryReader MReader(Data->ByteData);
			FObjectAndNameAsStringProxyArchive ProxObj(MReader, true);
			ProxObj.ArIsSaveGame = true;
			Actor->Serialize(ProxObj);
			
			Counter++;
			
			ISaveSystemInterface::Execute_OnLoadedFromSave(Actor);
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, FString::Printf(TEXT("Could not find \"%s\" in the provided array."), *Actor->GetFName().ToString()));
		}
	}

	OnFinishedLoading.Broadcast();
	return true;
}

bool USaveGame_SurvivalGame::ReadSaveGame()
{
	if (FullSlotName == "") return false;
	return ReadSaveGameFromSlot(FullSlotName);
}

bool USaveGame_SurvivalGame::ReadSaveGameFromSlot_Implementation(const FString& ToFullSlotName)
{
	FullSlotName = ToFullSlotName;
	if (!UGameplayStatics::DoesSaveGameExist(FullSlotName, 0)) {
		return true;
	}
	int32 Counter = 0;
	int32 FoundCount = 0;
	for (TActorIterator<AActor> Itr(GetWorld()); Itr; ++Itr) {
		//if (!IsValid(Itr)) continue;
		//FoundCount++;
		if (!Itr->GetClass()->ImplementsInterface(USaveSystemInterface::StaticClass())) {
			GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, FString::Printf(TEXT("%s does NOT implement."), *Itr->GetFName().ToString()));
			continue;
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Green, FString::Printf(TEXT("%s does implement."), *Itr->GetFName().ToString()));
		}
	}
	for (AActor* Actor : TActorRange<AActor>(GetWorld()))
	{
		if (!IsValid(Actor)) continue;
		FoundCount++;
		if (!Actor->GetClass()->ImplementsInterface(USaveSystemInterface::StaticClass())){
			//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, FString::Printf(TEXT("%s does NOT implement."), *Actor->GetFName().ToString()));
			continue;
		}

		FGenericActorSaveData* Data = SavedActorData.Find(Actor->GetFName());
		if (Data) {
			Actor->SetActorTransform(Data->Transform);

			FMemoryReader MReader(Data->ByteData);
			FObjectAndNameAsStringProxyArchive ProxObj(MReader, true);
			ProxObj.ArIsSaveGame = true;
			Actor->Serialize(ProxObj);

			Counter++;

			ISaveSystemInterface::Execute_OnLoadedFromSave(Actor);
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1,20,FColor::Red, FString::Printf(TEXT("Could not find \"%s\" in the provided array."), *Actor->GetFName().ToString()));
		}
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, FString::Printf(TEXT("Cycled through %s / %s actors"), *FString::FromInt(Counter), *FString::FromInt(FoundCount)));

	OnFinishedLoading.Broadcast();

	return true;
}

int32 USaveGame_SurvivalGame::GetCount() const
{
	return SavedActorData.Num();
}
