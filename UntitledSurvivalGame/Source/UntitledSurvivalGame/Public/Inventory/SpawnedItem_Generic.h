// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "SpawnedItem_Generic.generated.h"

class UItemData;

UCLASS()
class UNTITLEDSURVIVALGAME_API ASpawnedItem_Generic : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
	UItemData* ContainedItemData;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ItemMesh;
	
public:
	// Sets default values for this actor's properties
	ASpawnedItem_Generic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool CanInteract(const FInteractionInfo Info) const;
	virtual void BeginInteraction_Implementation(const FInteractionInfo Info);

};
