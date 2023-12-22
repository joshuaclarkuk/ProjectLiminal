// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/UniqueObjects/PeepHole.h"
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

		RotateCameraWithinPeepHole(HitResult, DeltaTime);
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

void APeepHole::RotateCameraWithinPeepHole(FHitResult& HitResult, float DeltaTime)
{
	// Rotate camera towards the light (mouse's impact point) up to a maximum angle
	FVector CameraLocation = CameraComponent->GetComponentLocation();
	FRotator DesiredRotation = FRotationMatrix::MakeFromX(HitResult.ImpactPoint - CameraLocation).Rotator();

	DesiredRotation.Pitch = FMath::ClampAngle(DesiredRotation.Pitch, -MaxCameraMovementAngle, MaxCameraMovementAngle);
	DesiredRotation.Yaw = FMath::ClampAngle(DesiredRotation.Yaw, -MaxCameraMovementAngle, MaxCameraMovementAngle);

	FRotator NewCameraRotation = FMath::RInterpTo(CameraComponent->GetComponentRotation(), DesiredRotation, DeltaTime, LookAroundInterpSpeed);

	CameraComponent->SetWorldRotation(NewCameraRotation);
}

void APeepHole::ReturnPlayerToFloor(AProjectLiminalCharacter* Player)
{
	PeepHoleState = EPS_NotActive;
	Torchlight->SetVisibility(false);

	Super::ReturnPlayerToFloor(Player);
}
