// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PowerSystemInterface.generated.h"

class UPowerSystemData;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UPowerSystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNTITLEDSURVIVALGAME_API IPowerSystemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/*
	*	Gets this object's linked power system. The uppermost node in a network owns this object.
	*	@return Returns the linked PowerSystemData. Should be set within APowerNetworkNode::ConstructPowerSystem.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Power System")
	UPowerSystemData* GetLinkedPowerSystem() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Power System")
	int32 GetGeneratedPowerUpstream() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Power System")
	int32 GetConsumedPowerDownstream() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Power System")
	AActor* GetUpstreamPowerProvider() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Power System")
	bool GetIsProvidingPower() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Power System")
	bool GetIsReceivingPower() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Power System")
	void UpdatePowerState(bool ForceUpdate = false);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Power System")
	int32 GetPower() const;
};
