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

void ACodeMachine::PressButton(int32 ButtonArrayValue)
{
	if (InventoryComponent)
	{
		bool bInventoryContainsItemToOpen = InventoryComponent->GetInventoryList().Contains(ItemRequiredToOpen);

		if (bInventoryContainsItemToOpen)
		{
			if (ArrayOfAttachedButtons[ButtonArrayValue])
			{
				// Assign attached button to an array index (0-2)
				APressableButton* PressableButton = CastChecked<APressableButton>(ArrayOfAttachedButtons[ButtonArrayValue]);
				if (PressableButton)
				{
					PressableButton->TriggerButton(ButtonArrayValue);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No buttons attached to code machine"))
			}
		}
		else
		{
			// Play error sound
		}
	}
}
