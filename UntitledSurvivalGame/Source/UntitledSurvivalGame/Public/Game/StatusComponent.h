// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStat_Int {
	GENERATED_BODY()

	FCharacterStat_Int()
	{};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Value;

	FCharacterStat_Int& operator+(FCharacterStat_Int& OtherStat) { Value += OtherStat.Value; return *this; }
	FCharacterStat_Int& operator+(int32 OtherStat) { Value += OtherStat; return *this; }
	FCharacterStat_Int& operator+(float OtherStat) { Value += OtherStat; return *this; }

	FCharacterStat_Int& operator-(FCharacterStat_Int& OtherStat) { Value -= OtherStat.Value; return *this; }
	FCharacterStat_Int& operator-(int32 OtherStat) { Value -= OtherStat; return *this; }
	FCharacterStat_Int& operator-(float OtherStat) { Value -= OtherStat; return *this; }

	//FCharacterStat_Int& operator=(FCharacterStat_Int& OtherStat) { Value = OtherStat.Value; return *this; }
	//FCharacterStat_Int& operator=(int32 OtherStat) { Value = OtherStat; return *this; }
	//FCharacterStat_Int& operator=(float OtherStat) { Value = OtherStat; return *this; }

	bool operator==(FCharacterStat_Int& OtherStat) { return (Name == OtherStat.Name); }
};


UCLASS( Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTITLEDSURVIVALGAME_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
