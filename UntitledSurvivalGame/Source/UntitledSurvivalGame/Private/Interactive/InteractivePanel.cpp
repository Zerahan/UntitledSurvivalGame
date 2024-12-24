// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactive/InteractivePanel.h"
#include "Camera/CameraComponent.h"
#include "Game/SurvivalCharacter.h"

// Sets default values
AInteractivePanel::AInteractivePanel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	CameraVisual = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraVisual->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AInteractivePanel::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInteractivePanel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

bool AInteractivePanel::CanInteract_Implementation(const FInteractionInfo Info) const
{
	return true;
}

void AInteractivePanel::BeginInteraction_Implementation(const FInteractionInfo Info)
{
	if(IsValid(InteractingPawnRef)){
		OnInteraction(Info);
	}else{
		if(IsValid(Info.PawnRef)){
			InteractingPawnRef = Cast<ASurvivalCharacter>(Info.PawnRef);
			if (IsValid(InteractingPawnRef)) {
				InteractingPawnRef->OverrideCameraView(this, true, true);
			}
		}
	}
}

void AInteractivePanel::CancelInteraction_Implementation(const FInteractionInfo Info)
{
	if (IsValid(InteractingPawnRef)) {
		InteractingPawnRef->OverrideCameraView(nullptr, false, false);
	}
	InteractingPawnRef = nullptr;
}

void AInteractivePanel::OnInteraction_Implementation(const FInteractionInfo Info)
{
}

