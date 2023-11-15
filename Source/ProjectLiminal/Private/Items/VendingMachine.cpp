// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/VendingMachine.h"
#include "Config/ProjectLiminalPlayerController.h"
#include "Items/Codes/CodeComponent.h"
#include "Items/Codes/PressableButton.h"
#include "Components/PointLightComponent.h"


AVendingMachine::AVendingMachine()
{
	CodeComponent = CreateDefaultSubobject<UCodeComponent>(TEXT("CodeComponent"));

	CodeIndicatorLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("CodeIndicatorComponent"));
	CodeIndicatorLight->SetupAttachment(ObjectMesh);
	CodeIndicatorLight->SetLightColor(FLinearColor::Red);
	CodeIndicatorLight->SetAttenuationRadius(3.0f);
	CodeIndicatorLight->SetIntensity(5000.0f);
}

void AVendingMachine::BeginPlay()
{
	Super::BeginPlay();

	ConstructPressableButtonArray();
}

void AVendingMachine::ConstructPressableButtonArray()
{
	// Get the array of child actors attached to this actor
	GetAttachedActors(ArrayOfAttachedButtons);
}

void AVendingMachine::PressButton(int32 ButtonArrayValue)
{
	if (ArrayOfAttachedButtons[ButtonArrayValue])
	{
		APressableButton* PressableButton = CastChecked<APressableButton>(ArrayOfAttachedButtons[ButtonArrayValue]);
		if (PressableButton)
		{
			PressableButton->TriggerButton(ButtonArrayValue);
		}
	}
}

