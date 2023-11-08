// Fill out your copyright notice in the Description page of Project Settings.


#include "Config/ProjectLiminalPlayerController.h"
#include "Characters/ProjectLiminalCharacter.h"
#include "Characters/ProjectLiminalCharacter.h"

void AProjectLiminalPlayerController::ZoomOnObjectAndEnableMouse(AActor* ObjectOfFocus, float CameraSnapSpeedInSeconds)
{
	SetViewTargetWithBlend(ObjectOfFocus, CameraSnapSpeedInSeconds); // Blend duration can be adjusted
	SetShowMouseCursor(true);
	bEnableClickEvents = true;
	UE_LOG(LogTemp, Warning, TEXT("The value of bEnableClickEvents is: %s"), bEnableClickEvents ? TEXT("true") : TEXT("false"));
}

void AProjectLiminalPlayerController::ZoomBackOutAndDisableMouse(AProjectLiminalCharacter* PlayerCharacter, float CameraSnapSpeedInSeconds)
{	
	bEnableClickEvents = false;
	SetShowMouseCursor(false);
	SetViewTargetWithBlend(PlayerCharacter, CameraSnapSpeedInSeconds); // Blend duration can be adjusted
	UE_LOG(LogTemp, Warning, TEXT("The value of bEnableClickEvents is: %s"), bEnableClickEvents ? TEXT("true") : TEXT("false"));
}


