// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/Power/PowerComponent_Basic.h"
#include "PowerComponent_Relay.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FPowerSwitch {
	GENERATED_BODY()

	FPowerSwitch()
		: TargetRef(nullptr)
		, State(false)
	{};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UPowerComponent_Basic* TargetRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool State;
};

UCLASS(Blueprintable, BlueprintType)
class UNTITLEDSURVIVALGAME_API UPowerComponent_Relay : public UPowerComponent_Basic
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool IsRelayActive;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UPowerComponent_Basic* SourceRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FPowerSwitch> Switches;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool UseDownstreamFlow;
	
	//UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	//TArray<bool> SwitchStates;
	
	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//TArray<UPowerComponent_Basic*> SwitchTargetRefs;
	
public:
	UPowerComponent_Relay();

	UFUNCTION(BlueprintCallable)
	void SetSwitchToState(int32 id, bool NewState);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateSwitchState(int32 SwitchID, bool NewState);
	virtual void UpdateSwitchState_Implementation(int32 SwitchID, bool NewState);

	UFUNCTION(BlueprintCallable)
	float GatherDownstreamPowerDraw();
};
