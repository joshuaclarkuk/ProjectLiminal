// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PeepHole.h"
#include "Components/PointLightComponent.h"

APeepHole::APeepHole()
{
	Torchlight = CreateDefaultSubobject<UPointLightComponent>(TEXT("TorchLight"));
	Torchlight->SetupAttachment(GetRootComponent());
	Torchlight->SetIntensity(TorchlightIntensity);
	Torchlight->SetAttenuationRadius(TorchlightAttenuationRadius);
}

void APeepHole::BeginPlay()
{
	Super::BeginPlay();
	Torchlight->SetVisibility(false);
}

void APeepHole::MovePlayerInFrontOfObject()
{
	Super::MovePlayerInFrontOfObject();
	if (bTorchlightIsNeeded)
	{
		Torchlight->SetVisibility(true);
	}
}
