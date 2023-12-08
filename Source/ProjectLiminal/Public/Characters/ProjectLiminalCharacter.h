// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ProjectLiminalCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;
class UAudioComponent;
class AInteractableBase;
class AProjectLiminalPlayerController;
class ACodeMachine;
class UInventoryComponent;
class ATicketDispenser;

UENUM()
enum EPlayerStates {
	EPS_Unoccupied,
	EPS_Interacting,
	EPS_InInventory,
	EPS_RelinquishControl
};

UCLASS(config=Game)
class AProjectLiminalCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	AProjectLiminalCharacter();

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	USceneComponent* LocationToDisplayInventoryItem;

	void ExitFromInteraction();

protected:
	virtual void BeginPlay();	

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	/** MappingContext */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Interact Input Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	/** Move Input Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PushButtonOneAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PushButtonTwoAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PushButtonThreeAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InventoryAction;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* FootstepsAudioComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
	USoundBase* FootstepMetaSound;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	UInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
	float MovementSpeedModifier = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
	float FootstepAudioInterval = 0.6f;

	EPlayerStates PlayerState = EPlayerStates::EPS_Unoccupied;
	AInteractableBase* CurrentInteractableObject;
	ACodeMachine* InteractableObjectIsCodeMachine;
	ATicketDispenser* InteractableObjectIsTicketDispenser;

	FTimerHandle FootstepAudioTimerHandle;
	FTimerHandle SweepTimerHandle;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void PushButtonOne();
	void PushButtonTwo();
	void PushButtonThree();
	void ReleaseButton();
	void Inventory();

	void PlayFootstepAudio();
	void SweepForInteractable();
	void InteractWithObject();

public:
	/*GETTERS & SETTERS*/

	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	// Get and Set Player State to prevent player movement
	EPlayerStates GetPlayerState() { return PlayerState; }
	void SetPlayerState(EPlayerStates NewState) { PlayerState = NewState; }

	// Get location to display inventory item (usable in UInventoryComponent)
	FVector GetLocationToDisplayInventoryItem() { return LocationToDisplayInventoryItem->GetComponentLocation(); }	
	FRotator GetRotationToDisplayInventoryItem() { return LocationToDisplayInventoryItem->GetComponentRotation(); }
};

