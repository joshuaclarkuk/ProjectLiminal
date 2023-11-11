// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/InteractableBase.h"
#include "PeepHole.generated.h"

class UPointLightComponent;

/**
 * 
 */
UCLASS()
class PROJECTLIMINAL_API APeepHole : public AInteractableBase
{
	GENERATED_BODY()

public:
	APeepHole();

protected:
	virtual void BeginPlay() override;
	virtual void MovePlayerInFrontOfObject() override;

	UPROPERTY(EditAnywhere)
	UPointLightComponent* Torchlight;

	UPROPERTY(EditAnywhere, Category = "Torchlight Settings")
	float TorchlightIntensity = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Torchlight Settings")
	float TorchlightAttenuationRadius = 60.0f;

	UPROPERTY(EditInstanceOnly, Category = "Torchlight Settings")
	bool bTorchlightIsNeeded = false;
};
