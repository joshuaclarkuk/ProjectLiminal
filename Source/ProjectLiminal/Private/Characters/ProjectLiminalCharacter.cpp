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
#include "Objects/InteractableBase.h"
#include "Objects/UniqueObjects/CodeMachine.h"
#include "Inventory/InventoryComponent.h"
#include "Objects/UniqueObjects/TicketDispenser.h"
#include "Kismet/GameplayStatics.h"

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

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	LocationToDisplayInventoryItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LocationToDisplayInventoryItem"));
	LocationToDisplayInventoryItem->SetupAttachment(FirstPersonCameraComponent);
	LocationToDisplayInventoryItem->SetRelativeLocation(FVector(70.0f, 0.0f, 0.0f));
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
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Started, this, &AProjectLiminalCharacter::Click);

		// Pushing Interactable Buttons
		EnhancedInputComponent->BindAction(PushLeftButtonAction, ETriggerEvent::Started, this, &AProjectLiminalCharacter::PushLeftButton);
		EnhancedInputComponent->BindAction(PushMiddleButtonAction, ETriggerEvent::Started, this, &AProjectLiminalCharacter::PushMiddleButton);
		EnhancedInputComponent->BindAction(PushRightButtonAction, ETriggerEvent::Started, this, &AProjectLiminalCharacter::PushRightButton);

		// Inventory
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &AProjectLiminalCharacter::Inventory);
	}
}


void AProjectLiminalCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (PlayerState == EPS_Unoccupied && Controller)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y * MovementSpeedModifier);
		AddMovementInput(GetActorRightVector(), MovementVector.X * MovementSpeedModifier);
	}
	// Feels massively clumsy but can't think of a better way to tackle movement by interp right now
	else if (PlayerState == EPS_InInventory && Controller && InventoryComponent && !InventoryComponent->GetIsScrolling())
	{
		// Protects against a bug where input on Y while scrolling caused the ScrollIndex to get confused
		// Have given a degree of tolerance for gamepads, but might need to adjust in case value is too large
		if (MovementVector.Y > 0.1f || MovementVector.Y < -0.1f) { return; }

		if (MovementVector.X < 0.0f)
		{
			InventoryComponent->SetScrollBehaviour(true, true);
		}
		else if (MovementVector.X > 0.1f)
		{
			InventoryComponent->SetScrollBehaviour(true, false);
		}
		else
		{
			InventoryComponent->SetScrollBehaviour(false, true);
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

void AProjectLiminalCharacter::PushLeftButton()
{
	if (PlayerState == EPS_Interacting && InteractableObjectIsCodeMachine)
	{
		InteractableObjectIsCodeMachine->PressButton(0);
	}
}

void AProjectLiminalCharacter::PushMiddleButton()
{
	if (PlayerState == EPS_Interacting && InteractableObjectIsCodeMachine)
	{
		InteractableObjectIsCodeMachine->PressButton(1);
	}
}

void AProjectLiminalCharacter::PushRightButton()
{
	if (PlayerState == EPS_Interacting && InteractableObjectIsCodeMachine)
	{
		InteractableObjectIsCodeMachine->PressButton(2);
	}
}

void AProjectLiminalCharacter::Inventory()
{
	if (PlayerState == EPS_Unoccupied)
	{
		InventoryComponent->OpenInventory();
	}
	else if (PlayerState == EPS_InInventory)
	{
		InventoryComponent->CloseInventory();
	}
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
				// Don't allow interaction if object has been "solved"
				if (CurrentInteractableObject->GetHasBeenSolved()) { return; }

				CurrentInteractableObject->SetInteractPromptVisibility(true);
				InteractableObjectIsCodeMachine = Cast<ACodeMachine>(CurrentInteractableObject);
				InteractableObjectIsTicketDispenser = Cast<ATicketDispenser>(CurrentInteractableObject);
			}
		}
		else
		{
			if (CurrentInteractableObject)
			{
				CurrentInteractableObject->SetInteractPromptVisibility(false);
			}
			CurrentInteractableObject = nullptr;
			InteractableObjectIsCodeMachine = nullptr;
			InteractableObjectIsTicketDispenser = nullptr;
		}
	}
}

void AProjectLiminalCharacter::InteractWithObject()
{
	if (CurrentInteractableObject && PlayerState == EPS_Unoccupied)
	{
		// Don't allow interaction if object has been "solved"
		if (CurrentInteractableObject->GetHasBeenSolved()) { return; }

		PlayerState = EPS_Interacting;
		CurrentInteractableObject->SetInteractPromptVisibility(false);
		CurrentInteractableObject->MovePlayerInFrontOfObject();
	}
	else if (CurrentInteractableObject && PlayerState == EPS_Interacting)
	{
		ExitFromInteraction();
	}
}

void AProjectLiminalCharacter::ExitFromInteraction()
{
	PlayerState = EPS_Unoccupied;
	CurrentInteractableObject->SetInteractPromptVisibility(true);
	CurrentInteractableObject->ReturnPlayerToFloor(this);
}
