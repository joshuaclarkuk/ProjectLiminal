// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LightGradualFadeComponent.h"
#include "Components/LightComponent.h"

// Sets default values for this component's properties
ULightGradualFadeComponent::ULightGradualFadeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULightGradualFadeComponent::BeginPlay()
{
	Super::BeginPlay();

	LightComponentToFade = GetOwner()->GetComponentByClass<ULightComponent>();
	if (LightComponentToFade)
	{
		LightComponentToFade->SetIntensity(StartingLightValue);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Light Component found by LightGradualFadeComponent on %s"), *GetOwner()->GetActorNameOrLabel());
	}

	// Stops Tick running when light is not changing
	SetComponentTickEnabled(false);
}

// Called every frame
void ULightGradualFadeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FadeToNewValue(DeltaTime);
}

void ULightGradualFadeComponent::ToggleLight(bool ShouldBeOn)
{
	SetComponentTickEnabled(true);

	bShouldBeOn = ShouldBeOn;
}

void ULightGradualFadeComponent::FadeToNewValue(float DeltaTime)
{
	if (bShouldBeOn)
	{
		float NewIntensity = FMath::FInterpTo(LightComponentToFade->Intensity, MaxLightValue, DeltaTime, FadeSpeed);
		LightComponentToFade->SetIntensity(NewIntensity);

		if (LightComponentToFade->Intensity >= MaxLightValue)
		{
			UE_LOG(LogTemp, Display, TEXT("LightFadeComponent no longer ticking"));
			SetComponentTickEnabled(false);
		}
	}
	else
	{
		float NewIntensity = FMath::FInterpTo(LightComponentToFade->Intensity, 0.0f, DeltaTime, FadeSpeed);
		LightComponentToFade->SetIntensity(NewIntensity);

		if (LightComponentToFade->Intensity <= 0.0f)
		{
			UE_LOG(LogTemp, Display, TEXT("LightFadeComponent no longer ticking"));
			SetComponentTickEnabled(false);
		}
	}
}

