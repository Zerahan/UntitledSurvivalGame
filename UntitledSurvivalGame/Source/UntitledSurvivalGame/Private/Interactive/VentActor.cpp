// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactive/VentActor.h"

// Sets default values
AVentActor::AVentActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVentActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVentActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

