// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/TicketDispenser.h"
#include "Components/SpotLightComponent.h"
#include "Math/UnrealMathUtility.h"

ATicketDispenser::ATicketDispenser()
{
	SmallOrangeLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SmallOrangeLight"));
	SmallOrangeLight->SetupAttachment(GetRootComponent());
	SmallOrangeLight->SetRelativeLocationAndRotation(FVector(0.886f, 0.0f, 34.5f),FRotator(-50.0f, 0.0f, 0.0f));
	SmallOrangeLight->SetIntensity(300.0f);
	SmallOrangeLight->SetAttenuationRadius(60.7f);
	SmallOrangeLight->SetInnerConeAngle(26.2f);
	SmallOrangeLight->SetOuterConeAngle(40.8f);
	SmallOrangeLight->bUseTemperature = true;
	SmallOrangeLight->SetTemperature(1600.0f);
}

void ATicketDispenser::BeginPlay()
{
	Super::BeginPlay();

	SmallOrangeLight->SetVisibility(false);	
	SmallOrangeLight->SetIntensity(0.0f);
}

void ATicketDispenser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SmallOrangeLight && bShouldTurnOn)
	{
		TurnOnLight(DeltaTime);
		UE_LOG(LogTemp, Warning, TEXT("Light Intensity: %f"), SmallOrangeLight->Intensity);
	}
}

void ATicketDispenser::TurnOnLight(float DeltaTime)
{
	SmallOrangeLight->SetVisibility(true);	

	float CurrentIntensity = SmallOrangeLight->Intensity;	
	float NewIntensity = FMath::FInterpConstantTo(CurrentIntensity, TargetLightIntensity, DeltaTime, LightInterpSpeed);
	SmallOrangeLight->SetIntensity(NewIntensity);

	if (NewIntensity >= TargetLightIntensity)
	{
		bShouldTurnOn = false;
	}
}
