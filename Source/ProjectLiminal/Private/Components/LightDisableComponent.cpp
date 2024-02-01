// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LightDisableComponent.h"
#include "Components/LightComponent.h"


// Sets default values for this component's properties
ULightDisableComponent::ULightDisableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void ULightDisableComponent::BeginPlay()
{
	Super::BeginPlay();

	LightComponentToDisable = GetOwner()->GetComponentByClass<ULightComponent>();
}


// Called every frame
void ULightDisableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULightDisableComponent::DisableLight()
{
	if (LightComponentToDisable)
	{
		if (bIsTurnedOff) { return; }

		LightComponentToDisable->GetOwner()->SetActorTickEnabled(false);
		LightComponentToDisable->GetOwner()->SetActorHiddenInGame(true);
		LightComponentToDisable->GetOwner()->SetActorEnableCollision(false);
		bIsTurnedOff = true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LightDisableComponent could not find light to disable. Please drag reference"));
	}
}

