// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PowerSystemData.generated.h"

class APowerNetworkNode;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdatedDispatcher);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPowerStateUpdatedDispatcher);

// All of the actors downstream of the controlled node found within the ControlNodes list.
USTRUCT(BlueprintType)
struct FDownstreamList {
	GENERATED_BODY()

	FDownstreamList()
	: DistanceFromTop(0)
	{};

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 DistanceFromTop;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AActor*> DownstreamActors;
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UNTITLEDSURVIVALGAME_API UPowerSystemData : public UObject
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> Generators;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> Consumers;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<APowerNetworkNode*> ControlNodes;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<FDownstreamList> ControlNodes_Downstream;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<int32> ControlNodes_SortedIndexes;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	int32 TotalGeneratedPower;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	int32 TotalConsumedPower;

public:
	UPowerSystemData();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Recalculate();
	virtual void Recalculate_Implementation();

	UFUNCTION(BlueprintCallable)
	void RegisterGenerator(AActor* ActorRef);

	UFUNCTION(BlueprintCallable)
	void UnregisterGenerator(AActor* ActorRef);

	UFUNCTION(BlueprintCallable)
	void RegisterConsumer(AActor* ActorRef);

	UFUNCTION(BlueprintCallable)
	void UnregisterConsumer(AActor* ActorRef);

	UFUNCTION(BlueprintCallable)
	void RegisterController(AActor* ActorRef);

	UFUNCTION(BlueprintCallable)
	void UnregisterController(AActor* ActorRef);

	UFUNCTION(BlueprintCallable)
	void RegisterWithSubnet(AActor* ActorRef);

	UFUNCTION(BlueprintCallable)
	void UnregisterWithSubnet(AActor* ActorRef);

	UFUNCTION(BlueprintCallable)
	void OnNodeStateUpdated(AActor* ActorRef);

	UFUNCTION(BlueprintCallable)
	void UpdateAllDownstreamNodesOf(APowerNetworkNode* Node);

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetDownstreamOf(AActor* ActorRef) const;

	UFUNCTION(BlueprintCallable)
	void OnFinishedLoad();

	UFUNCTION()
	void SortControlNodes();

	UPROPERTY(BlueprintAssignable)
	FOnUpdatedDispatcher OnUpdatedDispatcher;

	UPROPERTY(BlueprintAssignable)
	FOnPowerStateUpdatedDispatcher OnPowerStateUpdatedDispatcher;
};
