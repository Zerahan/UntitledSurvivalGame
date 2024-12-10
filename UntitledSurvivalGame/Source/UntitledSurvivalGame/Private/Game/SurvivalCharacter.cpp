// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SurvivalCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASurvivalCharacter::ASurvivalCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxInteractionOffset = 50;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	VaultDuration = 0.5f;

	QueryParams.AddIgnoredActor(this);
}

// Called when the game starts or when spawned
void ASurvivalCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	//GetWorldTimerManager().SetTimer(VaultTimerHandle, this, &ASurvivalCharacter::CheckCanVault_Timer, 0.1f, true, 2.0f);
}

// Called every frame
void ASurvivalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsVaulting) {
		VaultPercent = GetWorldTimerManager().GetTimerElapsed(VaultPathTimerHandle) / VaultDuration * 2;
		FVector Loc;
		if (VaultPercent < 1) {
			Loc = FMath::Lerp(GetActorLocation(), VaultPath[0], VaultPercent);
		}
		else {
			Loc = FMath::Lerp(GetActorLocation(), VaultPath[1], VaultPercent - 1);
		}
		SetActorLocation(Loc);
	}

	if (CanVault) {
		DrawDebugBox(GetWorld(), VaultLocation, FVector(1, 1, 1), FColor::Red);
		DrawDebugLine(GetWorld(), VaultLocation, VaultLocation + (FVector::UpVector * 5), FColor::Red);
		DrawDebugLine(GetWorld(), VaultLocation, VaultLocation + (VaultNormal * 5), FColor::Red);
	}
}

// Called to bind functionality to input
void ASurvivalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASurvivalCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASurvivalCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASurvivalCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASurvivalCharacter::Move(const FInputActionValue& Value)
{
	if (IsMoveInputIgnored()) return;
	if (IsVaulting) if (VaultPercent < 1.75f) return;
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ASurvivalCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
		SERVER_CameraPitch(GetControlRotation().Pitch);
	}
}

void ASurvivalCharacter::SERVER_CameraPitch_Implementation(float Pitch)
{
	MULTICAST_CameraPitch(Pitch);
}

void ASurvivalCharacter::MULTICAST_CameraPitch_Implementation(float Pitch)
{
	UpdateCameraPitch(Pitch);
}

void ASurvivalCharacter::UpdateCameraPitch_Implementation(float Pitch)
{
}

void ASurvivalCharacter::GetCameraLocationAndRotation_Implementation(FVector& Location, FRotator& Rotation)
{
	Location = GetCameraComponent()->GetComponentLocation();
	Rotation = GetCameraComponent()->GetComponentRotation();
}

void ASurvivalCharacter::Jump()
{
	if (IsMoveInputIgnored()) return;
	if (IsVaulting) return;
	CheckCanVault_Timer();
	if (!CanVault) {
		Super::Jump();
		return;
	}
	VaultPath.Empty();
	FVector OffsetTarget = VaultNormal * (GetCapsuleComponent()->GetScaledCapsuleRadius());
	OffsetTarget.Z = OffsetTarget.Z + GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 1;
	VaultPath.Add(VaultLocation + OffsetTarget);
	OffsetTarget = VaultNormal * (GetCapsuleComponent()->GetScaledCapsuleRadius()*-1);
	OffsetTarget.Z = OffsetTarget.Z + GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 1;
	VaultPath.Add(VaultLocation + OffsetTarget);
	IsVaulting = true;
	GetWorldTimerManager().SetTimer(VaultPathTimerHandle, this, &ASurvivalCharacter::StopVaulting, VaultDuration, false);
	//FVector(GetCapsuleComponent()->GetScaledCapsuleRadius(), 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	/*
	for (int i = 0; i < VaultPath.Num(); i++) {
		if (i == 0) {
			DrawDebugLine(GetWorld(), GetActorLocation(), VaultLocation + VaultPath[i], FColor::Red, false, 5);
			DrawDebugLine(GetWorld(), VaultLocation + VaultPath[i], VaultLocation + VaultPath[i+1], FColor::Red, false, 5);
		}
		DrawDebugBox(GetWorld(), VaultLocation + VaultPath[i], FVector::OneVector, FColor::Red, false, 5);
	}
	//*/
}

void ASurvivalCharacter::CheckCanVault_Timer()
{
	CanVault = false;
	if(IsVaulting) return;
	bool HasHit = false;
	FHitResult Hit;

	FCollisionQueryParams Query;
	Start = GetActorLocation() + GetActorRotation().RotateVector(FVector(0, 0, 120));
	End = GetActorLocation() + GetActorRotation().RotateVector(FVector(50, 0, 120));
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Camera);
	HasHit = Hit.bBlockingHit;
	if (HasHit) return;

	Hit = FHitResult();
	Start = GetActorLocation() + GetActorRotation().RotateVector(FVector(50, 0, 120));
	End = GetActorLocation() + GetActorRotation().RotateVector(FVector(50, 0, -50));
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Camera);
	HasHit = Hit.bBlockingHit;
	if (!HasHit) return;
	if ((Start - Hit.Location).Length() < 10) return;

	Start = FVector(GetActorLocation().X, GetActorLocation().Y,Hit.Location.Z + 2);
	End = FVector(Hit.Location.X, Hit.Location.Y, Hit.Location.Z + 2);
	TArray<TEnumAsByte<EObjectTypeQuery>> Objects;
	Objects.Add(TEnumAsByte(EObjectTypeQuery::ObjectTypeQuery1));
	TArray<AActor*, FDefaultAllocator>	ToIgnore;
	ToIgnore.Add(this);
	Hit = FHitResult();
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, 5, ETraceTypeQuery::TraceTypeQuery1, true, TArray<AActor*, FDefaultAllocator>(), EDrawDebugTrace::None, Hit, true);
	//UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), Start, End, 10, Objects, true, ToIgnore, EDrawDebugTrace::ForDuration, Hit, true);
	CanVault = Hit.bBlockingHit;
	if (!CanVault) return;
	VaultLocation = Hit.ImpactPoint;
	VaultNormal = Hit.ImpactNormal;
}

void ASurvivalCharacter::StopVaulting()
{
	IsVaulting = false;
}

void ASurvivalCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	CurrentInteractionInfo.ControllerRef = NewController;
	CurrentInteractionInfo.PawnRef = this;
	CurrentInteractionInfo.HitComponent = nullptr;
	GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Emerald, "PossessedBy");
}

void ASurvivalCharacter::UnPossessed()
{
	Super::UnPossessed();
	CurrentInteractionInfo.ControllerRef = nullptr;
	CurrentInteractionInfo.HitComponent = nullptr;
	GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Emerald, "UnPossessed");
}

void ASurvivalCharacter::StartInteraction_Implementation()
{
	UpdateCameraTrace();
	if (!IsValid(LastCameraTraceResult.GetActor())) return;
	if (!UKismetSystemLibrary::DoesImplementInterface(LastCameraTraceResult.GetActor(), UInteractionInterface::StaticClass())) { OnStartInteractionWithoutInterface(LastCameraTraceResult.GetActor()); LastCameraTraceResult.bBlockingHit = false; return; }
	FInteractionInfo Info = FInteractionInfo(CurrentInteractionInfo);
	Info.ControllerRef	= GetController();
	Info.PawnRef		= this;
	Info.HitComponent	= LastCameraTraceResult.GetComponent();
	//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Emerald, FString::Printf(TEXT("Controller: %s"), *GetController()->GetFName().ToString()));
	//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Emerald, FString::Printf(TEXT("Pawn: %s"), *GetFName().ToString()));
	//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Emerald, FString::Printf(TEXT("HitComponent: %s"), *LastCameraTraceResult.GetComponent()->GetFName().ToString()));
	CurrentInteractionInfo.HitComponent = LastCameraTraceResult.GetComponent();
	if (!IInteractionInterface::Execute_CanInteract(LastCameraTraceResult.GetActor(), Info)) { LastCameraTraceResult.bBlockingHit = false; return; }
	InteractionDistance = (FVector::Dist(GetCameraComponent()->GetComponentLocation(), LastCameraTraceResult.ImpactPoint)) + MaxInteractionOffset;
	IInteractionInterface::Execute_BeginInteraction(LastCameraTraceResult.GetActor(), Info);
	GetWorldTimerManager().SetTimer(InteractionCheckTimerHandle, this, &ASurvivalCharacter::CheckInteractionBreak, 0.1f, true);
}

void ASurvivalCharacter::OnStartInteractionWithoutInterface_Implementation(AActor* WithActor)
{
}

void ASurvivalCharacter::StopInteraction_Implementation()
{
	if (LastCameraTraceResult.bBlockingHit) {
		IInteractionInterface* Interface = Cast<IInteractionInterface>(LastCameraTraceResult.GetActor());
		if (UKismetSystemLibrary::DoesImplementInterface(LastCameraTraceResult.GetActor(), UInteractionInterface::StaticClass())) {
			IInteractionInterface::Execute_EndInteraction(LastCameraTraceResult.GetActor(), CurrentInteractionInfo);
			GetWorldTimerManager().ClearTimer(InteractionCheckTimerHandle);
		}
		else {
			OnStopInteractionWithoutInterface(LastCameraTraceResult.GetActor());
		}
	}
}

void ASurvivalCharacter::OnStopInteractionWithoutInterface_Implementation(AActor* WithActor)
{
}

void ASurvivalCharacter::CheckInteractionBreak_Implementation()
{
	if (FVector::Dist(GetCameraComponent()->GetComponentLocation(), LastCameraTraceResult.ImpactPoint) > InteractionDistance) {
		if (UKismetSystemLibrary::DoesImplementInterface(LastCameraTraceResult.GetActor(), UInteractionInterface::StaticClass())) {
			IInteractionInterface::Execute_CancelInteraction(LastCameraTraceResult.GetActor(), CurrentInteractionInfo);
			LastCameraTraceResult.bBlockingHit = false;
			GetWorldTimerManager().ClearTimer(InteractionCheckTimerHandle);
		}
	}
}

bool ASurvivalCharacter::UpdateCameraTrace(float MaxDistance)
{
	FVector CameraLocation	= GetCameraComponent()->GetComponentLocation();
	GetWorld()->LineTraceSingleByChannel(LastCameraTraceResult, CameraLocation, CameraLocation + (GetCameraComponent()->GetForwardVector() * MaxDistance), ECollisionChannel::ECC_Camera, QueryParams);
	//UKismetSystemLibrary::LineTraceSingle(GetWorld(), CameraLocation, CameraLocation + (GetCameraComponent()->GetForwardVector() * MaxDistance), ETraceTypeQuery::TraceTypeQuery2, false, {}, EDrawDebugTrace::None, LastCameraTraceResult, true, FLinearColor::Red, FLinearColor::Green, 2.f);
	return LastCameraTraceResult.bBlockingHit;
}

USceneComponent* ASurvivalCharacter::GetCameraComponent_Implementation() const
{
	return GetRootComponent();
}

