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

UENUM()
enum EPlayerStates {
	EPS_Unoccupied,
	EPS_Interacting,
	EPS_RelinquishControl
};

UCLASS(config=Game)
class AProjectLiminalCharacter : public ACharacter
{
	GENERATED_BODY()


	
public:
	AProjectLiminalCharacter();

protected:
	virtual void BeginPlay();	

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	/** MappingContext */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Interact Input Action */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	/** Move Input Action */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ClickAction;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
	float MovementSpeedModifier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
	float FootstepAudioInterval = 1.0f;

	EPlayerStates PlayerState = EPlayerStates::EPS_Unoccupied;
	AInteractableBase* CurrentInteractableObject;

	FTimerHandle FootstepAudioTimerHandle;
	FTimerHandle SweepTimerHandle;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void Click();

	void PlayFootstepAudio();
	void SweepForInteractable();
	void InteractWithObject();

public:
	/*GETTERS & SETTERS*/

	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	EPlayerStates GetPlayerState() { return PlayerState; }
};

