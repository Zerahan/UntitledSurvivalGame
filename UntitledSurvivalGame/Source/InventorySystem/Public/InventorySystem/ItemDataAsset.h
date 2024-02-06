// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class INVENTORYSYSTEM_API UItemDataAsset : public UDataAsset
{
	GENERATED_BODY()

	static const FText DEFAULT_NAME;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	FText DisplayName;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UTexture2D> Icon;
	
public:
	UItemDataAsset();

	UFUNCTION(BlueprintCallable)
	UTexture2D* GetIcon() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsItemValid() const;
	virtual bool IsItemValid_Implementation() const;
};
