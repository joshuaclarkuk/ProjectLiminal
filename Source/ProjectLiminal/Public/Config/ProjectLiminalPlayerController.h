// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectLiminalPlayerController.generated.h"

class AProjectLiminalCharacter;

/**
 * 
 */
UCLASS()
class PROJECTLIMINAL_API AProjectLiminalPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void ZoomOnObjectAndEnableMouse(AActor* ObjectOfFocus, float CameraSnapSpeedInSeconds, bool bEnableMouse);
	void ZoomBackOutAndDisableMouse(AProjectLiminalCharacter* PlayerCharacter, float CameraSnapSpeedInSeconds);

protected:
};
