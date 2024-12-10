// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ItemData.h"
#include "Inventory/ItemAsset_Generic.h"
#include "Inventory/SpawnedItem_Generic.h"
#include "Inventory/InventoryComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASpawnedItem_Generic::ASpawnedItem_Generic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (IsValid(ContainedItemData)) {
		ItemMesh->SetStaticMesh(ContainedItemData->GetStaticData()->SpawnedMesh.LoadSynchronous());
		ItemMesh->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void ASpawnedItem_Generic::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(ContainedItemData)) {
		ContainedItemData->Rename(nullptr, this);
		ItemMesh->SetStaticMesh(ContainedItemData->GetStaticData()->SpawnedMesh.LoadSynchronous());
		ItemMesh->SetSimulatePhysics(true);
	} else {
		//Destroy(this);
	}
}

// Called every frame
void ASpawnedItem_Generic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ASpawnedItem_Generic::CanInteract(const FInteractionInfo Info) const
{
	return ContainedItemData->IsItemValid();
}

void ASpawnedItem_Generic::BeginInteraction_Implementation(const FInteractionInfo Info)
{
	if (!IsValid(Info.PawnRef)) return;
	UInventoryComponent* InventoryRef = Info.PawnRef->GetComponentByClass<UInventoryComponent>();
	if (!IsValid(InventoryRef)) return;
	InventoryRef->AddItemToInventory(ContainedItemData);
	Destroy();
}

