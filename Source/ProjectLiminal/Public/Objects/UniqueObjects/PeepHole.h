// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/InteractableBase.h"
#include "PeepHole.generated.h"

class UPointLightComponent;
class UCameraComponent;
class AProjectLiminalPlayerController;

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

protected:
	virtual void BeginPlay() override;
	virtual void MovePlayerInFrontOfObject() override;
	virtual void ReturnPlayerToFloor(AProjectLiminalCharacter* Player) override;

	UPROPERTY(EditAnywhere)
	UPointLightComponent* Torchlight;

	UPROPERTY(EditAnywhere, Category = "Torchlight Settings")
	float TorchlightIntensity = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Torchlight Settings")
	float TorchlightAttenuationRadius = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Torchlight Settings")
	float LookAroundInterpSpeed = 0.6f;

	UPROPERTY(EditAnywhere, Category = "Torchlight Settings")
	float MaxCameraMovementAngle = 5.0f;

	UPROPERTY(EditInstanceOnly, Category = "Torchlight Settings")
	float TorchlightTraceDistance = 200.0f;

	UPROPERTY(EditInstanceOnly, Category = "Torchlight Settings")
	bool bTorchlightIsNeeded = false;

	EPeepHoleState PeepHoleState = EPeepHoleState::EPS_NotActive;
	AProjectLiminalPlayerController* PlayerControllerRef;
	UCameraComponent* CameraComponent;
};
