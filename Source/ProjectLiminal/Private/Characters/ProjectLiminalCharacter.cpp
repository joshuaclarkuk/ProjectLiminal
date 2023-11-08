// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/ProjectLiminalCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Sound/SoundBase.h"
#include "Engine/World.h"
#include "Items/InteractableBase.h"


//////////////////////////////////////////////////////////////////////////
// AProjectLiminalCharacter

AProjectLiminalCharacter::AProjectLiminalCharacter()
{	
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

	FootstepsAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FootstepsAudioComponent"));
	FootstepsAudioComponent->SetupAttachment(GetRootComponent());
}

void AProjectLiminalCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Start footstep audio
	GetWorldTimerManager().SetTimer(FootstepAudioTimerHandle, this, &AProjectLiminalCharacter::PlayFootstepAudio, FootstepAudioInterval, true);

	// Start sweep for interactables
	GetWorldTimerManager().SetTimer(SweepTimerHandle, this, &AProjectLiminalCharacter::SweepForInteractable, 0.1f, true);
}

//////////////////////////////////////////////////////////////////////////// Input

void AProjectLiminalCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProjectLiminalCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AProjectLiminalCharacter::Look);

		// Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AProjectLiminalCharacter::InteractWithObject);

		// Clicking
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AProjectLiminalCharacter::Click);
	}
}


void AProjectLiminalCharacter::Move(const FInputActionValue& Value)
{
	if (PlayerState == EPS_Unoccupied)
	{
		// input is a Vector2D
		FVector2D MovementVector = Value.Get<FVector2D>();

		if (Controller != nullptr)
		{
			// add movement 
			AddMovementInput(GetActorForwardVector(), MovementVector.Y * MovementSpeedModifier);
			AddMovementInput(GetActorRightVector(), MovementVector.X * MovementSpeedModifier);
		}
	}
}

void AProjectLiminalCharacter::Look(const FInputActionValue& Value)
{
	if (PlayerState == EPS_Unoccupied)
	{
		// input is a Vector2D
		FVector2D LookAxisVector = Value.Get<FVector2D>();

		if (Controller != nullptr)
		{
			// add yaw and pitch input to controller
			AddControllerYawInput(LookAxisVector.X);
			AddControllerPitchInput(-LookAxisVector.Y);
		}
	}
}

void AProjectLiminalCharacter::Jump()
{
	if (PlayerState == EPS_Unoccupied)
	{
		ACharacter::Jump();
	}
}

void AProjectLiminalCharacter::Click()
{

}

void AProjectLiminalCharacter::PlayFootstepAudio()
{
	FVector CharacterSpeed = AProjectLiminalCharacter::GetVelocity();

	if (CharacterSpeed.Size() > 0.05f && CharacterSpeed.Z == 0.0f)
	{
		FootstepsAudioComponent->Play();
	}
}

void AProjectLiminalCharacter::SweepForInteractable()
{
	if (PlayerState == EPS_Unoccupied)
	{
		FVector StartLocation = FirstPersonCameraComponent->GetComponentLocation();
		FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
		FVector EndLocation = StartLocation + (ForwardVector * 260.0f);

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;

		bool bHit = GetWorld()->SweepSingleByChannel(HitResult, StartLocation, EndLocation, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(5.0f), CollisionParams);

		if (bHit)
		{
			CurrentInteractableObject = Cast<AInteractableBase>(HitResult.GetActor());

			if (CurrentInteractableObject)
			{
				CurrentInteractableObject->SetInteractPromptVisibility(true);
			}
		}

		else
		{
			if (CurrentInteractableObject)
			{
				CurrentInteractableObject->SetInteractPromptVisibility(false);
			}
			CurrentInteractableObject = nullptr;
		}
	}
}

void AProjectLiminalCharacter::InteractWithObject()
{
	if (CurrentInteractableObject && PlayerState == EPS_Unoccupied)
	{
		PlayerState = EPS_Interacting;
		CurrentInteractableObject->SetInteractPromptVisibility(false);
		CurrentInteractableObject->MovePlayerInFrontOfObject();
	}
	else if (CurrentInteractableObject && PlayerState == EPS_Interacting)
	{
		PlayerState = EPS_Unoccupied;
		CurrentInteractableObject->SetInteractPromptVisibility(true);
		CurrentInteractableObject->ReturnPlayerToFloor(this);
	}
}
