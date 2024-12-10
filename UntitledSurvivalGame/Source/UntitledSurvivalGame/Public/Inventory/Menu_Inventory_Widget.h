// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Menu_Inventory_Widget.generated.h"

class UInventoryComponent;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class UNTITLEDSURVIVALGAME_API UMenu_Inventory_Widget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryRef;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* LinkedInventoryRef;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	int32 InventorySize;

public:
	UFUNCTION(BlueprintCallable)
	void SetInventoryRef(UInventoryComponent* NewInventoryRef);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void OnInventoryUpdated();
	virtual void OnInventoryUpdated_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnInventorySlotUpdated(int32 InventoryIndex, int32 SlotIndex);
	virtual void OnInventorySlotUpdated_Implementation(int32 InventoryIndex, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnLinkedInventoryUpdated(UInventoryComponent* NewLinkedInventoryRef);
	virtual void OnLinkedInventoryUpdated_Implementation(UInventoryComponent* NewLinkedInventoryRef);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetInventorySize(int32 NewSize);
	virtual void SetInventorySize_Implementation(int32 NewSize);
};
