// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Power/PowerComponent_Relay.h"

UPowerComponent_Relay::UPowerComponent_Relay()
{
	UseDownstreamFlow = true;
	IsRelayActive = true;
}

void UPowerComponent_Relay::SetSwitchToState(int32 id, bool NewState)
{
}

void UPowerComponent_Relay::UpdateSwitchState_Implementation(int32 SwitchID, bool NewState)
{
	if(!Switches.IsValidIndex(SwitchID)) return;
	if(Switches[SwitchID].State == NewState) return;
	Switches[SwitchID].State = NewState;

	PowerDraw = GatherDownstreamPowerDraw();
}

float UPowerComponent_Relay::GatherDownstreamPowerDraw()
{
	float TotalPowerDraw = 0.f;
	if (UseDownstreamFlow) {
		for (int32 i = 0; i < Switches.Num(); i++) {
			if (Switches[i].State) {
				TotalPowerDraw += Switches[i].TargetRef->GetPower();
			}
		}
	}
	else {
		if (SourceRef->GetClass()->IsChildOf(UPowerComponent_Relay::StaticClass())) {
			TotalPowerDraw += Cast<UPowerComponent_Relay>(SourceRef)->GatherDownstreamPowerDraw();
		} else {
			TotalPowerDraw += SourceRef->GetPower();
		}
	}
	return TotalPowerDraw;
}
