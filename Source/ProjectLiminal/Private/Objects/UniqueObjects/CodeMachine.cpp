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

ACodeMachine::ACodeMachine()
{
	CodeComponent = CreateDefaultSubobject<UCodeComponent>(TEXT("CodeComponent"));

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

	// Set up references to play and inventory to check whether player has required ticket to enter code
	PlayerCharacter = Cast<AProjectLiminalCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (PlayerCharacter)
	{
		InventoryComponent = PlayerCharacter->GetComponentByClass<UInventoryComponent>();
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
					PressableButton->TriggerButton(ButtonArrayValue);

					// Calculate value to add to code based on which button is pressed
					int32 ValueToAdd = (ButtonArrayValue + 1 + (ButtonArrayValue * ButtonArrayValue));
					CodeValueToEnter += ValueToAdd;

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

void ACodeMachine::RejectButtonPress()
{
	// Make light flash red
	// Play rejection noise
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Correct token needed"));
}

void ACodeMachine::EnterDigitToCode()
{
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
