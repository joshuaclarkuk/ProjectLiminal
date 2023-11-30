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

ACodeMachine::ACodeMachine()
{
	CodeComponent = CreateDefaultSubobject<UCodeComponent>(TEXT("CodeComponent"));

	AudioComponentHeader = CreateDefaultSubobject<USceneComponent>(TEXT("AudioComponentHeader"));
	AudioComponentHeader->SetupAttachment(GetRootComponent());

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
			AudioComponents[i]->FadeOut(0.0f, 0.0f);
		}
	}

	// Set up references to play and inventory to check whether player has required ticket to enter code
	PlayerCharacter = Cast<AProjectLiminalCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (PlayerCharacter)
	{
		InventoryComponent = PlayerCharacter->GetComponentByClass<UInventoryComponent>();
	}
}

void ACodeMachine::InitialiseAudioComponents()
{
	AudioComponents.SetNum(NumberOfAudioComponents);

	for (int32 i = 0; i < NumberOfAudioComponents; i++)
	{
		FString AudioComponentName = FString::Printf(TEXT("AudioComponent_%d"), i);
		AudioComponents[i] = CreateDefaultSubobject<UAudioComponent>(*AudioComponentName);
		AudioComponents[i]->SetupAttachment(AudioComponentHeader);
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
					// TODO: There's a bug where you can press the buttons before they've full risen and then don't press down, despite sound still working
					PressableButton->TriggerButton(ButtonArrayValue);

					// Calculate value to add to code based on which button is pressed
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
		PlaySound(CodeValue, 0);
		break;

	case 3:
		PlaySound(CodeValue, 1);
		break;

	case 7:
		PlaySound(CodeValue, 2);
		break;

	case 13:
		PlaySound(CodeValue, 3);
		break;

	case 4:
		PlaySound(CodeValue, 4);
		break;

	case 8:
		PlaySound(CodeValue, 5);
		break;

	case 10:
		PlaySound(CodeValue, 6);
		break;

	case 14:
		PlaySound(CodeValue, 7);
		break;

	case 16:
		PlaySound(CodeValue, 8);
		break;

	case 20:
		PlaySound(CodeValue, 9);
		break;

	case 11:
		PlaySound(CodeValue, 10);
		break;

	case 17:
		PlaySound(CodeValue, 11);
		break;

	case 21:
		PlaySound(CodeValue, 12);
		break;

	case 23:
		PlaySound(CodeValue, 13);
		break;

	case 24:
		PlaySound(CodeValue, 14);
		break;

	default:
		// Code to execute if CodeValue doesn't match any case
		UE_LOG(LogTemp, Warning, TEXT("Value is not 1, 3, 7 or 13"));
		break;
	}
}

void ACodeMachine::PlaySound(int32 CodeValue, int32 AudioComponentIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("CodeValue is: %d, AudioComponent is: %s"), CodeValue, *AudioComponents[AudioComponentIndex]->GetName());

	// Fade out currently playing sounds before starting next sound
	for (int i = 0; i < AudioComponents.Num(); i++)
	{
		AudioComponents[i]->FadeOut(SoundFadeOutTime, 0.0f);
	}

	// Activate audio component
	AudioComponents[AudioComponentIndex]->FadeIn(SoundFadeInTime);

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
