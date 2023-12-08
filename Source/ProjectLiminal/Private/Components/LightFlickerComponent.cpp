// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LightFlickerComponent.h"
#include "Components/LightComponent.h"


// Sets default values for this component's properties
ULightFlickerComponent::ULightFlickerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULightFlickerComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get reference to light component
	AffectedLight = GetOwner()->GetComponentByClass<ULightComponent>();
	if (!AffectedLight)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot find light component on %s"), *GetOwner()->GetActorNameOrLabel());
	}
}

// Called every frame
void ULightFlickerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SetNewLightIntensity(DeltaTime);
	SwitchLightOnAndOff(DeltaTime);
}

void ULightFlickerComponent::SetNewLightIntensity(float DeltaTime)
{
	float NewLightIntensity = FMath::FInterpTo(AffectedLight->Intensity, TargetLightIntensity, DeltaTime, 20.0f);
	AffectedLight->SetIntensity(NewLightIntensity);

	if (AffectedLight->Intensity == TargetLightIntensity)
	{
		TargetLightIntensity = FMath::RandRange(1.0f, 8.0f);
	}
}

void ULightFlickerComponent::SwitchLightOnAndOff(float DeltaTime)
{
	FlickerTimer += DeltaTime;

	if (FlickerTimer >= CurrentFlickerInterval && AffectedLight)
	{
		AffectedLight->SetVisibility(bLightIsActive);
		bLightIsActive = !bLightIsActive;
		CurrentFlickerInterval = FMath::RandRange(MinFlickerTime, MaxFlickerTime);
		FlickerTimer = 0.0f;
	}
}

