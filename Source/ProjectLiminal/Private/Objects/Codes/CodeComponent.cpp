// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Codes/CodeComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/MoveWithInterpComponent.h"
#include "Characters/ProjectLiminalCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Objects/InteractableBase.h"


// Sets default values for this component's properties
UCodeComponent::UCodeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UCodeComponent::BeginPlay()
{
	Super::BeginPlay();

	// Set correct indicator light
	CodeIndicatorLight = GetOwner()->FindComponentByClass<UPointLightComponent>();

	// Initialise arrays and grab Door Movers from relevant doors (remember to drag reference into Details panel)
	DoorMovementComponents.SetNum(DoorActorReferences.Num());
	for (int i = 0; i < DoorActorReferences.Num(); i++)
	{
		if (DoorActorReferences[i])
		{
			DoorMovementComponents[i] = DoorActorReferences[i]->FindComponentByClass<UMoveWithInterpComponent>();
		}
	}

	PlayerCharacter = Cast<AProjectLiminalCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void UCodeComponent::EnterDigitToCode(int32 i)
{
	if (bIsUnlocked) { return; }

	EnteredCode.Add(i);
	CheckEnteredCode();
}

void UCodeComponent::CheckEnteredCode()
{
	// Need to check digit by digit as they are entered, as I'm using sounds rather than numerical codes
	for (int32 i = 0; i < EnteredCode.Num(); i++)
	{
		if (EnteredCode[i] == CodeToOpen[i])
		{
			UE_LOG(LogTemp, Warning, TEXT("Correct number entered"));
			UE_LOG(LogTemp, Warning, TEXT("Element %d: %d"), i, EnteredCode[i]);

			// Code executes on correct code entry
			if (i == CodeToOpen.Num() - 1)
			{
				// Set "solved" boolean to true to prevent further interaction
				AInteractableBase* OwningActor = Cast<AInteractableBase>(GetOwner());
				if (OwningActor)
				{
					OwningActor->SetHasBeenSolved(true);
				}
				// Bool protects against multiple unlocks
				bIsUnlocked = true;
				UE_LOG(LogTemp, Warning, TEXT("Correct code entered! Unlocked"));

				// Open ALL doors associated with machine
				CodeIndicatorLight->SetLightColor(FLinearColor::Green);
				for (int j = 0; j < DoorMovementComponents.Num(); j++)
				{
					if (DoorMovementComponents[j])
					{
						DoorMovementComponents[j]->SetShouldRotate(true);
					}
				}

				// Exit out of interaction
				if (PlayerCharacter)
				{
					PlayerCharacter->ExitFromInteraction();
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Can't find %s"), *PlayerCharacter->GetName());
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Incorrect number entered. Clearing"));
			EnteredCode.Empty();
			return;
		}
	}
}

