// Fill out your copyright notice in the Description page of Project Settings.


#include "Config/ProjectLiminalPlayerController.h"
#include "Characters/ProjectLiminalCharacter.h"
#include "Characters/ProjectLiminalCharacter.h"

void AProjectLiminalPlayerController::ZoomOnObjectAndEnableMouse(AActor* ObjectOfFocus, float CameraSnapSpeedInSeconds, bool bEnableMouse)
{
	SetViewTargetWithBlend(ObjectOfFocus, CameraSnapSpeedInSeconds);
	SetShowMouseCursor(bEnableMouse);
	bEnableClickEvents = true;
}

void AProjectLiminalPlayerController::ZoomBackOutAndDisableMouse(AProjectLiminalCharacter* PlayerCharacter, float CameraSnapSpeedInSeconds)
{	
	bEnableClickEvents = false;
	SetShowMouseCursor(false);
	SetViewTargetWithBlend(PlayerCharacter, CameraSnapSpeedInSeconds);
}


