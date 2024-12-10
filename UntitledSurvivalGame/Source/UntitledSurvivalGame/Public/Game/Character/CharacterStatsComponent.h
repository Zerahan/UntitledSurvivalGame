// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatsComponent.generated.h"

UENUM(BlueprintType)
enum ECharacterStatType : uint8 {
	HEALTH	UMETA(DisplayName = "Health"),
	STAMINA	UMETA(DisplayName = "Stamina"),
	FOOD	UMETA(DisplayName = "Hunger"),
	WATER	UMETA(DisplayName = "Thirst"),
	MAX		UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(ECharacterStatType, ECharacterStatType::MAX)


USTRUCT(BlueprintType)
struct FCharacterStat_Basic {
	GENERATED_BODY()

	FCharacterStat_Basic()
		: Name(NAME_None)
		, Value(0)
		, Min(0)
		, Max(100)
		, IsDirty(false)
	{};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Value;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Min;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Max;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool IsDirty;

	bool IsValid() const {
		return Name != NAME_None;
	}

	bool GetIsDirty() const{
		return IsDirty;
	}
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTITLEDSURVIVALGAME_API UCharacterStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatsComponent();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TMap<TEnumAsByte<ECharacterStatType>, FCharacterStat_Basic> Stats;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	
	UFUNCTION(BlueprintCallable)
	FCharacterStat_Basic GetStat(ECharacterStatType Type) const;

	UFUNCTION(BlueprintCallable)
	TArray<FCharacterStat_Basic> GetStats() const;

	UFUNCTION(BlueprintCallable)
	TArray<FCharacterStat_Basic> GetChangedStats() const;

	UFUNCTION(BlueprintCallable)
	void MarkCleaned(ECharacterStatType Type);

	UFUNCTION(BlueprintCallable)
	float ModifyStat(ECharacterStatType Type, float Value, bool SetStat = false);
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool IsValidStat(FCharacterStat_Basic Value) const;
};
