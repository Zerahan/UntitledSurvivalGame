// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/InteractionInterface.h"
#include "SurvivalCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS(Blueprintable, BlueprintType)
class UNTITLEDSURVIVALGAME_API ASurvivalCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY()
	FTimerHandle VaultTimerHandle;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	bool CanVault;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	FVector VaultLocation;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	FVector VaultNormal;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	FVector Start;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	FVector End;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<FVector> VaultPath;

	UPROPERTY()
	float VaultDuration;

	UPROPERTY()
	FTimerHandle VaultPathTimerHandle;

	UPROPERTY()
	bool IsVaulting;

	UPROPERTY()
	float VaultPercent;

	UPROPERTY()
	FTimerHandle InteractionCheckTimerHandle;

	UPROPERTY()
	FInteractionInfo CurrentInteractionInfo;

	struct FCollisionQueryParams QueryParams;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	FHitResult LastCameraTraceResult;

	UPROPERTY()
	FVector InteractionLocation;

	UPROPERTY()
	float InteractionDistance;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float MaxInteractionOffset;

public:
	// Sets default values for this character's properties
	ASurvivalCharacter();

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	virtual void Jump() override;

	UFUNCTION(BlueprintCallable)
	void CheckCanVault_Timer();

	UFUNCTION()
	void StopVaulting();

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartInteraction();
	virtual void StartInteraction_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnStartInteractionWithoutInterface( AActor* WithActor);
	virtual void OnStartInteractionWithoutInterface_Implementation( AActor* WithActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StopInteraction();
	virtual void StopInteraction_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnStopInteractionWithoutInterface(AActor* WithActor);
	virtual void OnStopInteractionWithoutInterface_Implementation(AActor* WithActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CheckInteractionBreak();
	virtual void CheckInteractionBreak_Implementation();

	UFUNCTION(BlueprintCallable)
	bool UpdateCameraTrace(float MaxDistance = 200.f);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	USceneComponent* GetCameraComponent() const;
	virtual USceneComponent* GetCameraComponent_Implementation() const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Server, Unreliable)
	void SERVER_CameraPitch(float Pitch);
	virtual void SERVER_CameraPitch_Implementation(float Pitch);

	UFUNCTION(NetMulticast, Unreliable)
	void MULTICAST_CameraPitch(float Pitch);
	virtual void MULTICAST_CameraPitch_Implementation(float Pitch);

	UFUNCTION(BlueprintNativeEvent)
	void UpdateCameraPitch(float Pitch);
	virtual void UpdateCameraPitch_Implementation(float Pitch);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GetCameraLocationAndRotation(FVector& Location, FRotator& Rotation);
	virtual void GetCameraLocationAndRotation_Implementation(FVector& Location, FRotator& Rotation);

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};
