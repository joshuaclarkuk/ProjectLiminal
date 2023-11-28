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
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "Objects/Codes/CodeSoundComponent.h"

ACodeMachine::ACodeMachine()
{
	CodeComponent = CreateDefaultSubobject<UCodeComponent>(TEXT("CodeComponent"));

	InitialiseCodeSounds();
	InitialiseAudioComponents();

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

	// Stop keys all playing a sound on start
	for (int i = 0; i < NumberOfAudioComponents; i++)
	{
		if (AudioComponents[i])
		{
			AudioComponents[i]->Stop();
		}
	}

	// Set up references to play and inventory to check whether player has required ticket to enter code
	PlayerCharacter = Cast<AProjectLiminalCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (PlayerCharacter)
	{
		InventoryComponent = PlayerCharacter->GetComponentByClass<UInventoryComponent>();
	}
}

void ACodeMachine::InitialiseCodeSounds()
{
	CodeSounds.SetNum(NumberOfAudioComponents);

	// Assuming the sound path is correct
	static ConstructorHelpers::FObjectFinder<USoundBase> SoundCueFinder(TEXT("/Game/Audio/SFX/Buttons/Alligator_Bass_Note_Cue.Alligator_Bass_Note_Cue"));

	if (SoundCueFinder.Succeeded())
	{
		// The sound was found, assign it to all elements in the array
		for (int32 i = 0; i < NumberOfAudioComponents; i++)
		{
			CodeSounds[i] = SoundCueFinder.Object;
		}
	}
	else
	{
		// Handle the case where the sound was not found
		UE_LOG(LogTemp, Error, TEXT("Sound not found!"));
	}
}

void ACodeMachine::InitialiseAudioComponents()
{
	AudioComponents.SetNum(NumberOfAudioComponents);

	for (int32 i = 0; i < NumberOfAudioComponents; i++)
	{
		FString AudioComponentName = FString::Printf(TEXT("AudioComponent_%d"), i);
		AudioComponents[i] = CreateDefaultSubobject<UAudioComponent>(*AudioComponentName);
		AudioComponents[i]->SetSound(CodeSounds[i]);
	}
}

void ACodeMachine::ConstructPressableButtonArray()
{
	// Get the array of child actors attached to this actor
	GetAttachedActors(ArrayOfAttachedButtons);
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
					PressableButton->TriggerButton(ButtonArrayValue);

					// Calculate value to add to code based on which button is pressed
					int32 ValueToAdd = (ButtonArrayValue + 1 + (ButtonArrayValue * ButtonArrayValue));
					CodeValueToEnter += ValueToAdd;

					// Use total code value to determine which note/chord to play
					SelectAndPlaySound(CodeValueToEnter);

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

void ACodeMachine::SelectAndPlaySound(int32 CodeValue)
{
	switch (CodeValue)
	{
	case 1:
		UE_LOG(LogTemp, Warning, TEXT("Value is 1"));
		AudioComponents[0]->Play();
		break;

	case 3:
		UE_LOG(LogTemp, Warning, TEXT("Value is 3"));
		AudioComponents[1]->Play();
		break;

	case 7:
		UE_LOG(LogTemp, Warning, TEXT("Value is 7"));
		AudioComponents[2]->Play();
		break;

	case 13:
		UE_LOG(LogTemp, Warning, TEXT("Value is 13"));
		AudioComponents[3]->Play();
		break;

	case 4:
		UE_LOG(LogTemp, Warning, TEXT("Value is 4"));
		AudioComponents[4]->Play();
		break;

	default:
		// Code to execute if Value doesn't match any case
		UE_LOG(LogTemp, Warning, TEXT("Value is not 1, 3, 7 or 13"));
		break;
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
