// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Interfaces/PowerSystemInterface.h"
#include "PowerNetworkNode.generated.h"

class UPowerSystemData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPowerStateUpdated);

UCLASS(Blueprintable)
class UNTITLEDSURVIVALGAME_API APowerNetworkNode : public AActor, public IInteractionInterface, public IPowerSystemInterface
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool IsSwitchOn;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	bool IsReceivingPower;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	AActor* PowerProviderRef;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UPowerSystemData* LinkedPowerSystem;
	
public:	
	// Sets default values for this actor's properties
	APowerNetworkNode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetLinkedPowerSystem(UPowerSystemData* System);
	virtual void SetLinkedPowerSystem_Implementation(UPowerSystemData* System);

	virtual UPowerSystemData* GetLinkedPowerSystem_Implementation() const;

	virtual bool GetIsProvidingPower_Implementation() const;
	virtual bool GetIsReceivingPower_Implementation() const;

	virtual AActor* GetUpstreamPowerProvider_Implementation() const;
	virtual void UpdatePowerState_Implementation(bool ForceUpdate = false);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetPower(int32 Value);
	virtual void SetPower_Implementation(int32 Value);
	
	UFUNCTION(BlueprintCallable)
	bool GetIsSwitchOn() const;

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	//bool GetCanChangeSwitch() const;
	//virtual bool GetCanChangeSwitch_Implementation() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateHasPower(bool State);
	virtual void UpdateHasPower_Implementation(bool State);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ForceSwitchOff(const int32 ID);
	virtual void ForceSwitchOff_Implementation(const int32 ID);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetIsSwitchOn(bool State);
	virtual void SetIsSwitchOn_Implementation(bool State);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateGraphics();
	virtual void UpdateGraphics_Implementation();

	virtual bool CanInteract_Implementation(const FInteractionInfo Info) const;
	virtual void BeginInteraction_Implementation(const FInteractionInfo Info);
	virtual void CancelInteraction_Implementation(const FInteractionInfo Info);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnBeginInteraction(const FInteractionInfo Info);
	virtual void OnBeginInteraction_Implementation(const FInteractionInfo Info);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ConstructPowerSystem();
	virtual void ConstructPowerSystem_Implementation();

	UFUNCTION(BlueprintCallable)
	APowerNetworkNode* GetHighestNodeFrom(AActor* Node, int32 Distance) const;

	UPROPERTY(BlueprintAssignable)
	FOnPowerStateUpdated OnStateUpdated;
};
