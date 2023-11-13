// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PeepHole.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Config/ProjectLiminalPlayerController.h"

APeepHole::APeepHole()
{
	Torchlight = CreateDefaultSubobject<UPointLightComponent>(TEXT("TorchLight"));
	Torchlight->SetupAttachment(GetRootComponent());
	Torchlight->SetIntensity(TorchlightIntensity);
	Torchlight->SetAttenuationRadius(TorchlightAttenuationRadius);
	Torchlight->SetSourceRadius(50.0f);
	Torchlight->SetSoftSourceRadius(20.0f);

}

void APeepHole::BeginPlay()
{
	Super::BeginPlay();

	CameraComponent = GetComponentByClass<UCameraComponent>();
	PlayerControllerRef = Cast<AProjectLiminalPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	if (PlayerControllerRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Got player controller in Peephole"));
	}

	Torchlight->SetVisibility(false);
}

void APeepHole::Tick(float DeltaTime)
{
	// Only ticks if PeepHole is currently active
	if (PeepHoleState == EPS_Active && PlayerControllerRef)
	{
		Super::Tick(DeltaTime);

		// Move torchlight along with mouse cursor via interp
		FHitResult HitResult;
		PlayerControllerRef->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		FVector NewTorchLocation = FMath::VInterpTo(Torchlight->GetComponentLocation(), HitResult.ImpactPoint, DeltaTime, 10.0f);
		Torchlight->SetWorldLocation(NewTorchLocation);
	}
}

void APeepHole::MovePlayerInFrontOfObject()
{
	Super::MovePlayerInFrontOfObject();

	PeepHoleState = EPS_Active;
	if (bTorchlightIsNeeded)
	{
		Torchlight->SetVisibility(true);
	}
}

void APeepHole::ReturnPlayerToFloor(AProjectLiminalCharacter* Player)
{
	PeepHoleState = EPS_NotActive;
	Torchlight->SetVisibility(false);

	Super::ReturnPlayerToFloor(Player);
}
