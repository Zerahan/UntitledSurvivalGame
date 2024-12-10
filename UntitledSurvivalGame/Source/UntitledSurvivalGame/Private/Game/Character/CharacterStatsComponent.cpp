// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/CharacterStatsComponent.h"

// Sets default values for this component's properties
UCharacterStatsComponent::UCharacterStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCharacterStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	for (ECharacterStatType Type : TEnumRange<ECharacterStatType>()) {
		FCharacterStat_Basic Stat = FCharacterStat_Basic();
		FText EnumDisplayName;
		UEnum::GetDisplayValueAsText(Type, EnumDisplayName);
		Stat.Name = FName(EnumDisplayName.ToString());
		Stats.Add(Type, Stat);
	}
}


FCharacterStat_Basic UCharacterStatsComponent::GetStat(ECharacterStatType Type) const
{
	if (Stats.Contains(Type)) {
		return Stats[Type];
	}
    return FCharacterStat_Basic();
}

TArray<FCharacterStat_Basic> UCharacterStatsComponent::GetStats() const
{
	TArray<FCharacterStat_Basic> Arr;
	Stats.GenerateValueArray(Arr);
	return Arr;
}

TArray<FCharacterStat_Basic> UCharacterStatsComponent::GetChangedStats() const
{
	TArray<FCharacterStat_Basic> Arr;
	for (ECharacterStatType Type : TEnumRange<ECharacterStatType>()) {
		if (Stats[Type].GetIsDirty()) {
			Arr.Add(Stats[Type]);
		}
	}
	return Arr;
}

void UCharacterStatsComponent::MarkCleaned(ECharacterStatType Type)
{
	Stats[Type].IsDirty = false;
}

float UCharacterStatsComponent::ModifyStat(ECharacterStatType Type, float Value, bool SetStat)
{
	float Rem = 0;
	float Old = float(Stats[Type].Value);
	if (SetStat) {
		if (Value > Stats[Type].Max) Rem = Value - Stats[Type].Max;
		if (Value < Stats[Type].Min) Rem = Value - Stats[Type].Max;
		Stats[Type].Value = FMath::Clamp(Value, Stats[Type].Min, Stats[Type].Max);
	}
	else {
		if (Stats[Type].Value + Value > Stats[Type].Max) Rem = Value - (Stats[Type].Max - Stats[Type].Value);
		if (Stats[Type].Value + Value < Stats[Type].Min) Rem = Value + (Stats[Type].Value - Stats[Type].Min);
		Stats[Type].Value = FMath::Clamp(Stats[Type].Value + Value, Stats[Type].Min, Stats[Type].Max);
	}
	if(Old != Stats[Type].Value) Stats[Type].IsDirty = true;
	return Rem;
}

// Called every frame
void UCharacterStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UCharacterStatsComponent::IsValidStat(FCharacterStat_Basic Value) const
{
	return Value.IsValid();
}

