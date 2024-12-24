// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "InteractivePanel.generated.h"

class UCameraComponent;
class ASurvivalCharacter;

UCLASS(Abstract, BlueprintType, Blueprintable)
class UNTITLEDSURVIVALGAME_API AInteractivePanel : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	ASurvivalCharacter* InteractingPawnRef;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraVisual;
	
public:	
	// Sets default values for this actor's properties
	AInteractivePanel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool CanInteract_Implementation(const FInteractionInfo Info) const;
	virtual void BeginInteraction_Implementation(const FInteractionInfo Info);
	virtual void CancelInteraction_Implementation(const FInteractionInfo Info);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnInteraction(const FInteractionInfo Info);
	void OnInteraction_Implementation(const FInteractionInfo Info);

};
