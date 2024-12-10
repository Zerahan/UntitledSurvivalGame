// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PowerComponent_Basic.generated.h"


UCLASS(Blueprintable, BlueprintType, Abstract, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNTITLEDSURVIVALGAME_API UPowerComponent_Basic : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float PowerDraw;

public:	
	// Sets default values for this component's properties
	UPowerComponent_Basic();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	float GetPower() const;
};
