// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Power/PowerNetworkNode.h"

// Sets default values
APowerNetworkNode::APowerNetworkNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APowerNetworkNode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APowerNetworkNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

