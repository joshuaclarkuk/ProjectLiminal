// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/UniqueObjects/CodeMachine.h"
#include "Config/ProjectLiminalPlayerController.h"
#include "Objects/Codes/CodeComponent.h"
#include "Objects/Codes/PressableButton.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Inventory/InventoryComponent.h"
#include "Characters/ProjectLiminalCharacter.h"
#include "Inventory/Items/ItemBase.h"
#include "Components/AudioComponent.h"
#include "Objects/Codes/CodeSoundComponent.h"
#include "Components/TextRenderComponent.h"
#include "NiagaraComponent.h"


ACodeMachine::ACodeMachine()
{
	InitialiseButtonPlacement();
	InitialiseAudioComponents();
	InitialiseCodeSymbolStaticMeshes();

	CodeComponent = CreateDefaultSubobject<UCodeComponent>(TEXT("CodeComponent"));

	KeyCodeAmbientParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("KeyCodeAmbientParticles"));
	KeyCodeAmbientParticles->SetupAttachment(GetRootComponent());

	KeyCodeSymbolParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("KeyCodeSymbolParticles"));
	KeyCodeSymbolParticles->SetupAttachment(ActiveSymbolStaticMesh);

	CodeIndicatorLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("CodeIndicatorComponent"));
	CodeIndicatorLight->SetupAttachment(ObjectMesh);
	CodeIndicatorLight->SetLightColor(FLinearColor::Red);
	CodeIndicatorLight->SetAttenuationRadius(3.0f);
	CodeIndicatorLight->SetIntensity(5000.0f);
}

void ACodeMachine::BeginPlay()
{
	Super::BeginPlay();

	ConstructPressableButtonArray();
	SilenceAudioComponentsOnStart();

	// Set up references to play and inventory to check whether player has required ticket to enter code
	PlayerCharacter = Cast<AProjectLiminalCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (PlayerCharacter)
	{
		InventoryComponent = PlayerCharacter->GetComponentByClass<UInventoryComponent>();
	}

	// Make sure particles don't start playing on start
	KeyCodeAmbientParticles->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	KeyCodeAmbientParticles->SetRelativeLocation(FVector(20.0f, 0.0f, 10.0f));
	KeyCodeAmbientParticles->Deactivate();
}

void ACodeMachine::InitialiseButtonPlacement()
{
	ButtonSpawnLocations.SetNum(NumberOfButtonsToSpawn);

	for (int32 i = 0; i < NumberOfButtonsToSpawn; i++)
	{
		FString ButtonLocationName = FString::Printf(TEXT("Button_%d"), i);
		ButtonSpawnLocations[i] = CreateDefaultSubobject<USceneComponent>(*ButtonLocationName);
		ButtonSpawnLocations[i]->SetupAttachment(GetRootComponent());
		ButtonSpawnLocations[i]->SetRelativeLocation(FVector(0.0f, 6.0f - (i * 6), 2.0f));
	}
}

void ACodeMachine::ConstructPressableButtonArray()
{
	// Spawn number of buttons and attach to correct scene component
	for (int i = 0; i < NumberOfButtonsToSpawn; i++)
	{
		if (ButtonClassToSpawn && ButtonSpawnLocations[i])
		{
			FActorSpawnParameters SpawnParams;
			APressableButton* NewButtonActor = GetWorld()->SpawnActor<APressableButton>(ButtonClassToSpawn, ButtonSpawnLocations[i]->GetComponentTransform(), SpawnParams);

			if (NewButtonActor)
			{
				ArrayOfAttachedButtons.Add(NewButtonActor);
			}
		}
	}
}

void ACodeMachine::InitialiseAudioComponents()
{
	AudioComponentHeader = CreateDefaultSubobject<USceneComponent>(TEXT("AudioComponentHeader"));
	AudioComponentHeader->SetupAttachment(GetRootComponent());

	AudioComponents.SetNum(NumberOfButtonCombinations);

	for (int32 i = 0; i < NumberOfButtonCombinations; i++)
	{
		FString AudioComponentName = FString::Printf(TEXT("AudioComponent_%d"), i);
		AudioComponents[i] = CreateDefaultSubobject<UAudioComponent>(*AudioComponentName);
		AudioComponents[i]->SetupAttachment(AudioComponentHeader);
	}
}

void ACodeMachine::InitialiseCodeSymbolStaticMeshes()
{
	ActiveSymbolStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ActiveSymbolStaticMesh"));
	ActiveSymbolStaticMesh->SetupAttachment(GetRootComponent());
	ActiveSymbolStaticMesh->SetVisibility(false);

	CodeSymbolHeader = CreateDefaultSubobject<USceneComponent>(TEXT("CodeSymbolHeader"));
	CodeSymbolHeader->SetupAttachment(GetRootComponent());

	CodeSymbolMeshes.SetNum(NumberOfButtonCombinations);

	for (int32 i = 0; i < NumberOfButtonCombinations; i++)
	{
		FString StaticMeshComponentName = FString::Printf(TEXT("CodeSymbolStaticMesh_%d"), i);
		CodeSymbolMeshes[i] = CreateDefaultSubobject<UStaticMeshComponent>(*StaticMeshComponentName);
		CodeSymbolMeshes[i]->SetupAttachment(CodeSymbolHeader);
	}
}

void ACodeMachine::SilenceAudioComponentsOnStart()
{
	// Stop keys all playing a sound on start
	for (int i = 0; i < NumberOfButtonCombinations; i++)
	{
		if (AudioComponents[i])
		{
			AudioComponents[i]->FadeOut(0.0f, 0.0f);
		}
	}
}

void ACodeMachine::MovePlayerInFrontOfObject()
{
	Super::MovePlayerInFrontOfObject();

	// "Wakes Up" audio components so the sound fades in on first key press. Sounds jarring otherwise.
	for (UAudioComponent* AudioComponent : AudioComponents)
	{
		if (AudioComponent)
		{
			AudioComponent->FadeIn(0.0f, 0.0f);
		}
	}
}

void ACodeMachine::ReturnPlayerToFloor(AProjectLiminalCharacter* Player)
{
	Super::ReturnPlayerToFloor(Player);
	for (UAudioComponent* AudioComponent : AudioComponents)
	{
		if (AudioComponent)
		{
			AudioComponent->FadeOut(SoundFadeOutTime, 0.0f);
		}
	}
}

void ACodeMachine::AttemptButtonPress(int32 ButtonArrayValue)
{
	if (InventoryComponent)
	{
		bool bInventoryContainsItemToOpen = InventoryComponent->GetInventoryList().Contains(ItemRequiredToOpen);

		if (bInventoryContainsItemToOpen || ItemRequiredToOpen == nullptr)
		{
			if (ArrayOfAttachedButtons[ButtonArrayValue])
			{
				// Assign attached button to an array index (0-2)
				APressableButton* PressableButton = CastChecked<APressableButton>(ArrayOfAttachedButtons[ButtonArrayValue]);
				if (PressableButton)
				{
					// Activate button animation
					// TODO: There's a bug where you can press the buttons before they've full risen and then don't press down, despite sound still working
					PressableButton->TriggerButton(ButtonArrayValue);

					// Calculate value to add to code based on which button is pressed
					// This is a slightly weird formula to ensure coded inputs don't return same value as other combinations
					int32 ValueToAdd = (ButtonArrayValue + 1 + (ButtonArrayValue * ButtonArrayValue));
					CodeValueToEnter += ValueToAdd;

					// Use total code value to determine which note/chord to play
					SelectSound(CodeValueToEnter);

					// Display code on screen
					FString DigitBeingAdded = FString::Printf(TEXT("Code to be input: %d"), ValueToAdd);
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, DigitBeingAdded);					
					FString TotalAmountToAdd = FString::Printf(TEXT("Total value is: %d"), CodeValueToEnter);
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, TotalAmountToAdd);
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("No buttons attached to code machine"))
			}
		}
		else
		{
			// Play error sound
			if (CorrectTicketMissingAudio)
			{
				UGameplayStatics::PlaySoundAtLocation(this, CorrectTicketMissingAudio, GetActorLocation());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Cannot find audio in: %s"), *GetName());
			}
		}
	}
}

void ACodeMachine::SelectSound(int32 CodeValue)
{
	switch (CodeValue)
	{
	case 1:
		PlaySoundAndDisplayGlyph(CodeValue, 0);
		break;

	case 3:
		PlaySoundAndDisplayGlyph(CodeValue, 1);
		break;

	case 7:
		PlaySoundAndDisplayGlyph(CodeValue, 2);
		break;

	case 4:
		PlaySoundAndDisplayGlyph(CodeValue, 3);
		break;

	case 8:
		PlaySoundAndDisplayGlyph(CodeValue, 4);
		break;

	case 10:
		PlaySoundAndDisplayGlyph(CodeValue, 5);
		break;

	case 11:
		PlaySoundAndDisplayGlyph(CodeValue, 6);
		break;

	default:
		// Code to execute if CodeValue doesn't match any case
		UE_LOG(LogTemp, Warning, TEXT("Value is not 1, 3, 7 or 13"));
		break;
	}
}

void ACodeMachine::PlaySoundAndDisplayGlyph(int32 CodeValue, int32 Index)
{
	UE_LOG(LogTemp, Warning, TEXT("CodeValue is: %d, AudioComponent is: %s"), CodeValue, *AudioComponents[Index]->GetName());

	// Fade out currently playing sounds before starting next sound
	for (int i = 0; i < AudioComponents.Num(); i++)
	{
		if (AudioComponents[i])
		{
			AudioComponents[i]->FadeOut(SoundFadeOutTime, 0.0f);
		}
	}

	// Activate new audio component
	if (AudioComponents[Index])
	{
		AudioComponents[Index]->FadeIn(SoundFadeInTime);
	}

	// Update Static Mesh and set visiblity to false (just to be sure)
	if (ActiveSymbolStaticMesh && CodeSymbolMeshes[Index])
	{
		ActiveSymbolStaticMesh->SetStaticMesh(CodeSymbolMeshes[Index]->GetStaticMesh());
		ActiveSymbolStaticMesh->SetVisibility(false);
	}

	// Play Niagara Systems
	if (KeyCodeAmbientParticles)
	{
		KeyCodeAmbientParticles->SetActive(true);
	}
}

void ACodeMachine::RejectButtonPress()
{
	// Make light flash red
	// Play rejection noise
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Correct token needed"));
}

void ACodeMachine::EnterDigitToCode()
{
	// Raise keys via interpolation
	// Raises all keys at once, even if being pressed. 
	// This eliminates confusion with partial codes
	for (int i = 0; i < ArrayOfAttachedButtons.Num(); i++)
	{
		APressableButton* PressableButton = Cast<APressableButton>(ArrayOfAttachedButtons[i]);
		if (PressableButton)
		{
			PressableButton->SetButtonIsRising(true);
		}
	}

	// Fades out all audio components
	for (int i = 0; i < AudioComponents.Num(); i++)
	{
		AudioComponents[i]->FadeOut(SoundFadeOutTime, 0.0f);
	}

	// Deactivates Particle System effect
	KeyCodeAmbientParticles->SetActive(false);

	// Protects against value being added when multiple notes are released
	if (CodeValueToEnter == 0) { return; }

	// Enter digit to code
	CodeComponent->EnterDigitToCode(CodeValueToEnter);
	FString DigitBeingAdded = FString::Printf(TEXT("Value added to code: %d"), CodeValueToEnter);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, DigitBeingAdded);

	// Clear digit so that player has to play new chord
	CodeValueToEnter = 0;
	UE_LOG(LogTemp, Warning, TEXT("CodeValueToEnter Has Been Reset To: %d"), CodeValueToEnter);

}
