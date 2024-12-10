// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PoweredMenu_Widget.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Abstract)
class UNTITLEDSURVIVALGAME_API UPoweredMenu_Widget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
	AActor* ParentActorRef;

	public:
	UFUNCTION(BlueprintCallable)
	AActor* GetParentActorRef() const;
};
