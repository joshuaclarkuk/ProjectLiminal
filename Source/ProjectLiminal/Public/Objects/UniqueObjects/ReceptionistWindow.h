// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/InteractableBase.h"
#include "ReceptionistWindow.generated.h"

class ULevelSequence;
class ULevelSequencePlayer;
class AProjectLiminalPlayerController;
class AProjectLiminalPlayerController;

UENUM()
enum class ESequenceState {
	ESS_Playing,
	ESS_NotPlaying
};

UCLASS()
class PROJECTLIMINAL_API AReceptionistWindow : public AInteractableBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void MovePlayerInFrontOfObject() override;

	UPROPERTY(EditAnywhere)
	ULevelSequence* BangOnWindowCameraSequence;
	ULevelSequencePlayer* LevelSequencePlayer;
	ESequenceState SequenceState = ESequenceState::ESS_NotPlaying;
	bool bHasPlayed = false;
	FTimerHandle LevelSequenceFinishedTimerHandle;

	AProjectLiminalPlayerController* PlayerController;
	AProjectLiminalCharacter* PlayerPawn;

	void PlayLevelSequence();
	void CheckLevelSequenceFinished();
};
