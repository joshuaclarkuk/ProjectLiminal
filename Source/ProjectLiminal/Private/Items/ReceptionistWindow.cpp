// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ReceptionistWindow.h"
#include "MovieSceneSequencePlayer.h"
#include "LevelSequencePlayer.h"
#include "LevelSequence.h"
#include "Engine/World.h"
#include "Config/ProjectLiminalPlayerController.h"
#include "Characters/ProjectLiminalCharacter.h"
#include "Kismet/GameplayStatics.h"

void AReceptionistWindow::BeginPlay()
{
	Super::BeginPlay();

    PlayerController = Cast<AProjectLiminalPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    PlayerPawn = Cast<AProjectLiminalCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void AReceptionistWindow::MovePlayerInFrontOfObject()
{
	Super::MovePlayerInFrontOfObject();
    PlayLevelSequence();
}

void AReceptionistWindow::PlayLevelSequence()
{
	if (BangOnWindowCameraSequence && SequenceState == ESequenceState::ESS_NotPlaying && !bHasPlayed)
	{
        ESequenceState::ESS_Playing;
        bHasPlayed = true;

        // Create a Level Sequence player
        ALevelSequenceActor* OutActor;  // Declare the OutActor parameter
        LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), BangOnWindowCameraSequence, FMovieSceneSequencePlaybackSettings(), OutActor);

        if (LevelSequencePlayer && PlayerController && PlayerPawn)
        {
            // Restrict Player Movement
            PlayerPawn->DisableInput(PlayerController);

            // Play the Level Sequence
            LevelSequencePlayer->Play();

            // Set up a timer to check if the sequence has finished
            GetWorld()->GetTimerManager().SetTimer(LevelSequenceFinishedTimerHandle, this, &AReceptionistWindow::CheckLevelSequenceFinished, 0.1f, true);
        }
    }
}

void AReceptionistWindow::CheckLevelSequenceFinished()
{
    // Check if the Level Sequence has finished playing
    if (LevelSequencePlayer && !LevelSequencePlayer->IsPlaying())
    {
        // Re-enable Player Input
        PlayerPawn->EnableInput(PlayerController);

        // Stop the timer
        GetWorld()->GetTimerManager().ClearTimer(LevelSequenceFinishedTimerHandle);

        // Clean up the Level Sequence player
        LevelSequencePlayer->Stop();
        LevelSequencePlayer->MarkAsGarbage();
        LevelSequencePlayer = nullptr;
    }
}
