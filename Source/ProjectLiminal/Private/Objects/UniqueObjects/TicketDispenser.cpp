// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/UniqueObjects/TicketDispenser.h"
#include "Components/SpotLightComponent.h"
#include "Math/UnrealMathUtility.h"

ATicketDispenser::ATicketDispenser()
{
	SmallOrangeLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SmallOrangeLight"));
	SmallOrangeLight->SetupAttachment(GetRootComponent());
	SmallOrangeLight->SetRelativeLocationAndRotation(FVector(0.886f, 0.0f, 34.5f),FRotator(-50.0f, 0.0f, 0.0f));
	SmallOrangeLight->SetIntensity(300.0f);
	SmallOrangeLight->SetAttenuationRadius(100.0f);
	SmallOrangeLight->SetInnerConeAngle(26.2f);
	SmallOrangeLight->SetOuterConeAngle(80.0f);
	SmallOrangeLight->bUseTemperature = true;
	SmallOrangeLight->SetTemperature(1600.0f);
}

void ATicketDispenser::BeginPlay()
{
	Super::BeginPlay();

	// Set timer to flicker light
	GetWorldTimerManager().SetTimer(LightFadeTimerHandle, this, &ATicketDispenser::MakeLightFlicker, FMath::FRandRange(0.1f, 0.4f), true);

	SmallOrangeLight->SetVisibility(false);	
	SmallOrangeLight->SetIntensity(MaxLightIntensity);
}

void ATicketDispenser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SmallOrangeLight && bShouldTurnOn)
	{
		MakeLightFade(DeltaTime);
	}
}

void ATicketDispenser::MakeLightFade(float DeltaTime)
{
	float CurrentIntensity = SmallOrangeLight->Intensity;	
	float NewIntensity = FMath::FInterpConstantTo(CurrentIntensity, TargetLightIntensity, DeltaTime, LightInterpSpeed);
	SmallOrangeLight->SetIntensity(NewIntensity);
}

void ATicketDispenser::MakeLightFlicker()
{
	if (bShouldTurnOn)
	{
		if (bShouldFlickerOn)
		{
			SmallOrangeLight->SetVisibility(true);
			bShouldFlickerOn = false;
		}
		else
		{
			SmallOrangeLight->SetVisibility(false);
			bShouldFlickerOn = true;
		}

		FlickerInterval = FMath::FRandRange(0.1f, 1.0f);
		TargetLightIntensity = FMath::FRandRange(MaxLightIntensity / 2, MaxLightIntensity);
		GetWorldTimerManager().SetTimer(LightFadeTimerHandle, this, &ATicketDispenser::MakeLightFlicker, FlickerInterval, true);
	}
}
