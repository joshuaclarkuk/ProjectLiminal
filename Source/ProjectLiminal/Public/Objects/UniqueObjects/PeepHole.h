// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/InteractableBase.h"
#include "PeepHole.generated.h"

class UPointLightComponent;
class UCameraComponent;
class AProjectLiminalPlayerController;
class UAudioComponent;
class USoundBase;

UENUM()
enum EPeepHoleState {
	EPS_NotActive,
	EPS_ZoomingIn,
	EPS_Active
};

/**
 * 
 */
UCLASS()
class PROJECTLIMINAL_API APeepHole : public AInteractableBase
{
	GENERATED_BODY()

public:
	APeepHole();
	virtual void Tick(float DeltaTime) override;

	void RotateCameraWithinPeepHole(FHitResult& HitResult, float DeltaTime);

protected:
	virtual void BeginPlay() override;
	virtual void MovePlayerInFrontOfObject() override;
	virtual void ReturnPlayerToFloor(AProjectLiminalCharacter* Player) override;

	UPROPERTY(EditAnywhere)
	UPointLightComponent* Torchlight;

	UPROPERTY(EditAnywhere)
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, Category = "Torchlight Settings")
	float TorchlightIntensity = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Torchlight Settings")
	float TorchlightAttenuationRadius = 100.0f;

	UPROPERTY(EditInstanceOnly, Category = "Torchlight Settings")
	float TorchlightTraceDistance = 200.0f;

	UPROPERTY(EditInstanceOnly, Category = "Torchlight Settings")
	bool bTorchlightIsNeeded = false;

	UPROPERTY(EditInstanceOnly, Category = "Torchlight Settings")
	USoundBase* TorchClickSound;

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	FVector StartingCameraLocation;

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	FRotator StartingCameraRotation;

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float LookAroundInterpSpeed = 0.6f;

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float MaxCameraMovementAngle = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float StartingSpringArmLength = 32.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float ZoomedInSpringArmLength = -1.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float ZoomSpeed = 3.0f;

	EPeepHoleState PeepHoleState = EPeepHoleState::EPS_NotActive;
	AProjectLiminalPlayerController* PlayerControllerRef;
	UCameraComponent* CameraComponent;
};
