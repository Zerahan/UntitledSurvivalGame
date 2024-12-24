// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Power/PowerSystemData.h"
#include "Game/Power/PowerNetworkNode.h"
#include "Interfaces/PowerSystemInterface.h"
/*
* Requirements:
*	If a node is switched on and the power draw is too high, it switches itself off.
*/

UPowerSystemData::UPowerSystemData()
{
}

void UPowerSystemData::Recalculate_Implementation()
{
	// Get all of the possible power the network can make.
	TotalGeneratedPower = 0;
	for (AActor* Ref : Generators) {
		if(!Ref->GetClass()->ImplementsInterface(UPowerSystemInterface::StaticClass())) continue;
		//IPowerSystemInterface* PoweredRef = Cast<IPowerSystemInterface>(Ref);
		if (!IPowerSystemInterface::Execute_GetIsProvidingPower(Ref)) continue;
		TotalGeneratedPower += IPowerSystemInterface::Execute_GetPower(Ref);
	}

	// Propagate downstream. Compare against the total generated. If greater, shut off the node.
	// There is definitely a more efficient way to do this, but I just need to get it working and move on.
	TotalConsumedPower = 0;
	for (int32 i = 0; i < ControlNodes_SortedIndexes.Num(); i++) {
		AActor* CurrentNode = Cast<AActor>(ControlNodes[ControlNodes_SortedIndexes[i]]);
		TArray<AActor*> Downstream = GetDownstreamOf(CurrentNode);
		int32 CurrentConsumed = 0;
		for (AActor* Ref : Downstream) {
			if (!Ref->GetClass()->ImplementsInterface(UPowerSystemInterface::StaticClass())) continue;
			if (!IPowerSystemInterface::Execute_GetIsProvidingPower(Ref)) continue;
			// Only add if below zero.
			CurrentConsumed += FMath::Min(0, IPowerSystemInterface::Execute_GetPower(Ref));
		}
		if (TotalGeneratedPower + TotalConsumedPower + CurrentConsumed >= 0) {
			TotalConsumedPower += CurrentConsumed;
		} else {
			// If adding the new consumed power would overwhelm the generators, shut off the node.
			APowerNetworkNode* Node = Cast<APowerNetworkNode>(CurrentNode);
			if (IsValid(Node)) {
				Node->SetIsSwitchOn(false);
			}
		}
	}
	OnPowerStateUpdatedDispatcher.Broadcast();
}

void UPowerSystemData::RegisterGenerator(AActor* ActorRef)
{
	int32 Index = Generators.AddUnique(ActorRef);
	if(Index == INDEX_NONE) return;
	Recalculate();
	OnUpdatedDispatcher.Broadcast();
	RegisterWithSubnet(ActorRef);
}

void UPowerSystemData::UnregisterGenerator(AActor* ActorRef)
{
	int32 Index = Generators.Find(ActorRef);
	if (Index == INDEX_NONE) return;
	Generators.RemoveAt(Index);
	Recalculate();
	OnUpdatedDispatcher.Broadcast();
	UnregisterWithSubnet(ActorRef);
}

void UPowerSystemData::RegisterConsumer(AActor* ActorRef)
{
	int32 Index = Consumers.AddUnique(ActorRef);
	if (Index == INDEX_NONE) return;
	Recalculate();
	OnUpdatedDispatcher.Broadcast();
	RegisterWithSubnet(ActorRef);
}

void UPowerSystemData::UnregisterConsumer(AActor* ActorRef)
{
	int32 Index = Consumers.Find(ActorRef);
	if (Index == INDEX_NONE) return;
	Consumers.RemoveAt(Index);
	Recalculate();
	OnUpdatedDispatcher.Broadcast();
	UnregisterWithSubnet(ActorRef);
}

void UPowerSystemData::RegisterController(AActor* ActorRef)
{
	if (!ActorRef->GetClass()->IsChildOf<APowerNetworkNode>()) return;
	int32 Index = ControlNodes.AddUnique(Cast<APowerNetworkNode>(ActorRef));
	if (Index == INDEX_NONE) return;
	// Build subnet for new controller
	TArray<AActor*> AllRegisteredNodes;
	AllRegisteredNodes.Append(Generators);
	AllRegisteredNodes.Append(Consumers);
	AllRegisteredNodes.Append(ControlNodes);
	ControlNodes_Downstream.EmplaceAt(Index, FDownstreamList());
	for (AActor* Ref : AllRegisteredNodes) {
		if(!IsValid(Ref)) continue;
		if(!Ref->GetClass()->ImplementsInterface(UPowerSystemInterface::StaticClass())) continue;
		AActor* ParentRef = IPowerSystemInterface::Execute_GetUpstreamPowerProvider(Ref);
		if(ParentRef != ActorRef) continue;
		ControlNodes_Downstream[Index].DownstreamActors.AddUnique(Ref);
	}
	RegisterWithSubnet(ActorRef);
	SortControlNodes();
	Recalculate();
	OnUpdatedDispatcher.Broadcast();
}

void UPowerSystemData::UnregisterController(AActor* ActorRef)
{
	if (!ActorRef->GetClass()->IsChildOf<APowerNetworkNode>()) return;
	int32 Index = ControlNodes.Find(Cast<APowerNetworkNode>(ActorRef));
	if (Index == INDEX_NONE) return;
	ControlNodes.RemoveAt(Index);
	ControlNodes_Downstream.RemoveAt(Index);
	UnregisterWithSubnet(ActorRef);
	SortControlNodes();
	Recalculate();
	OnUpdatedDispatcher.Broadcast();
}

void UPowerSystemData::RegisterWithSubnet(AActor* ActorRef)
{
	if (!ActorRef->GetClass()->ImplementsInterface(UPowerSystemInterface::StaticClass())) return;
	AActor* ParentRef = IPowerSystemInterface::Execute_GetUpstreamPowerProvider(ActorRef);
	if (!IsValid(ParentRef)) return;
	int32 Index = ControlNodes.Find(Cast<APowerNetworkNode>(ParentRef));
	if (Index == INDEX_NONE) return;
	if (!ControlNodes_Downstream.IsValidIndex(Index)) return;
	ControlNodes_Downstream[Index].DownstreamActors.AddUnique(ActorRef);
}

void UPowerSystemData::UnregisterWithSubnet(AActor* ActorRef)
{
	if (!ActorRef->GetClass()->ImplementsInterface(UPowerSystemInterface::StaticClass())) return;
	AActor* ParentRef = IPowerSystemInterface::Execute_GetUpstreamPowerProvider(ActorRef);
	if (!IsValid(ParentRef)) return;
	int32 Index = ControlNodes.Find(Cast<APowerNetworkNode>(ParentRef));
	if (Index == INDEX_NONE) return;
	if (!ControlNodes_Downstream.IsValidIndex(Index)) return;
	ControlNodes_Downstream[Index].DownstreamActors.Remove(ActorRef);
}

void UPowerSystemData::OnNodeStateUpdated(AActor* ActorRef)
{
	if (ActorRef->GetClass()->IsChildOf<APowerNetworkNode>()) {
		UpdateAllDownstreamNodesOf(Cast<APowerNetworkNode>(ActorRef));
		Recalculate();
	}
	else {
		if (!ActorRef->GetClass()->ImplementsInterface(UPowerSystemInterface::StaticClass())) return;
		AActor* ParentRef = IPowerSystemInterface::Execute_GetUpstreamPowerProvider(ActorRef);
		if(!IsValid(ParentRef)) return;
		if (!ActorRef->GetClass()->ImplementsInterface(UPowerSystemInterface::StaticClass())) return;
		IPowerSystemInterface::Execute_UpdatePowerState(ActorRef, false);
	}
}

void UPowerSystemData::UpdateAllDownstreamNodesOf(APowerNetworkNode* Node)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, FString::Printf(TEXT("Updating downstream of %s."), *Node->GetName()));
	int32 Index = ControlNodes.Find(Node);
	if (Index == INDEX_NONE) return;
	for (AActor* Ref : ControlNodes_Downstream[Index].DownstreamActors) {
		if (!Ref->GetClass()->ImplementsInterface(UPowerSystemInterface::StaticClass())) continue;
		IPowerSystemInterface::Execute_UpdatePowerState(Ref, true);
		/*
		if (Ref->GetClass()->IsChildOf<APowerNetworkNode>()) {
			UpdateAllDownstreamNodesOf(Cast<APowerNetworkNode>(Ref));
		}
		//*/
	}
}

TArray<AActor*> UPowerSystemData::GetDownstreamOf(AActor* ActorRef) const
{
	if (!ActorRef->GetClass()->IsChildOf(APowerNetworkNode::StaticClass())) return TArray<AActor*>();
	int32 Index = ControlNodes.Find(Cast<APowerNetworkNode>(ActorRef));
	if(Index == INDEX_NONE) return TArray<AActor*>();
	return ControlNodes_Downstream[Index].DownstreamActors;
}

void UPowerSystemData::SortControlNodes()
{
	ControlNodes_SortedIndexes.Empty();
	TArray<AActor*> NodesToCheck;
	APowerNetworkNode* CurrentNode = Cast<APowerNetworkNode>(GetOuter());
	for (int32 i = 0; i < ControlNodes.Num(); i++) {
		if (!IsValid(CurrentNode)) break;
		int32 Index = ControlNodes.Find(CurrentNode);
		if(Index == INDEX_NONE) break;
		ControlNodes_SortedIndexes.Add(Index);
		TArray<AActor*> Downstream = GetDownstreamOf(CurrentNode);
		for (AActor* Ref : Downstream) {
			if (ControlNodes.Contains(Ref)) {
				NodesToCheck.Add(Ref);
			}
		}
		if(NodesToCheck.Num() == 0) return;
		CurrentNode = Cast<APowerNetworkNode>(NodesToCheck[0]);
		NodesToCheck.RemoveAt(0);
	}
}
