// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/Power/PowerNetworkNode.h"
#include "Game/Power/PowerSystemData.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APowerNetworkNode::APowerNetworkNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APowerNetworkNode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APowerNetworkNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APowerNetworkNode::SetLinkedPowerSystem_Implementation(UPowerSystemData* System)
{
	if(IsValid(LinkedPowerSystem)) return;
	LinkedPowerSystem = System;
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Linking %s to %s"), *GetName(), *LinkedPowerSystem->GetOuter()->GetName()));
}

UPowerSystemData* APowerNetworkNode::GetLinkedPowerSystem_Implementation() const
{
	return LinkedPowerSystem;
}

bool APowerNetworkNode::GetIsProvidingPower_Implementation() const
{
	return IPowerSystemInterface::Execute_GetIsReceivingPower(this) && IsSwitchOn;
}

bool APowerNetworkNode::GetIsReceivingPower_Implementation() const
{
	return IsReceivingPower;
}

AActor* APowerNetworkNode::GetUpstreamPowerProvider_Implementation() const
{
	return PowerProviderRef;
}

void APowerNetworkNode::UpdatePowerState_Implementation(bool ForceUpdate)
{
	bool HasUpdated = ForceUpdate;
	if (IsValid(PowerProviderRef)) {
		if (PowerProviderRef->GetClass()->ImplementsInterface(UPowerSystemInterface::StaticClass())) {
			bool State = IPowerSystemInterface::Execute_GetIsProvidingPower(PowerProviderRef);
			if (IsReceivingPower != State) HasUpdated = true;
			IsReceivingPower = State;
		}
	}
	UpdateGraphics();
	if(HasUpdated && IsValid(LinkedPowerSystem)) LinkedPowerSystem->OnNodeStateUpdated(this);
}

void APowerNetworkNode::SetPower_Implementation(int32 Value)
{
}

bool APowerNetworkNode::GetIsSwitchOn() const
{
	return IsSwitchOn;
}

void APowerNetworkNode::SetIsSwitchOn_Implementation(bool State)
{
	if(IsSwitchOn == State) return;
	IsSwitchOn = State;
	IPowerSystemInterface::Execute_UpdatePowerState(this, true);
	UpdateGraphics();
}

void APowerNetworkNode::UpdateGraphics_Implementation()
{
}

bool APowerNetworkNode::CanInteract_Implementation(const FInteractionInfo Info) const
{
	return true;
}

void APowerNetworkNode::BeginInteraction_Implementation(const FInteractionInfo Info)
{
}

void APowerNetworkNode::CancelInteraction_Implementation(const FInteractionInfo Info)
{
}

void APowerNetworkNode::OnBeginInteraction_Implementation(const FInteractionInfo Info)
{
}

void APowerNetworkNode::ConstructPowerSystem_Implementation()
{
	// Only works if this node doesn't have an upstream.
	if(IsValid(PowerProviderRef)) return;
	LinkedPowerSystem = NewObject<UPowerSystemData>(this);
	//SetLinkedPowerSystem(NewObject<UPowerSystemData>(this));
	TArray<AActor*> TempNodes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APowerNetworkNode::StaticClass(), TempNodes);
	for (AActor* Ref : TempNodes) {
		if (!IsValid(Ref)) continue;
		APowerNetworkNode* Highest = GetHighestNodeFrom(Ref, 0);
		if (!IsValid(Highest)) continue;
		if (Highest == this) {
			Cast<APowerNetworkNode>(Ref)->SetLinkedPowerSystem(LinkedPowerSystem);
		}
	}
	for (AActor* Ref : TempNodes) {
		if (!IsValid(Ref)) continue;
		IPowerSystemInterface::Execute_UpdatePowerState(Ref, false);
	}
}

APowerNetworkNode* APowerNetworkNode::GetHighestNodeFrom(AActor* Node, int32 Distance) const
{
	if (Distance > 50){
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("%s: Max distance reached, is there a loop?"), *GetName()));
		return nullptr;
	}
	AActor* ParentNode = IPowerSystemInterface::Execute_GetUpstreamPowerProvider(Node);
	if (IsValid(ParentNode)) {
		return GetHighestNodeFrom(ParentNode, Distance + 1);
	}
	return Cast<APowerNetworkNode>(Node);
}

