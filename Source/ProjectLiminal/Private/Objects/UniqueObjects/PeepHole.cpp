// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/UniqueObjects/PeepHole.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Config/ProjectLiminalPlayerController.h"

APeepHole::APeepHole()
{
	Torchlight = CreateDefaultSubobject<UPointLightComponent>(TEXT("TorchLight"));
	Torchlight->SetupAttachment(GetRootComponent());
	Torchlight->SetIntensity(TorchlightIntensity);
	Torchlight->SetAttenuationRadius(TorchlightAttenuationRadius);
	Torchlight->SetSourceRadius(50.0f);
	Torchlight->SetSoftSourceRadius(20.0f);

	InteractCamSpringArm->TargetArmLength = StartingSpringArmLength;
}

void APeepHole::BeginPlay()
{
	Super::BeginPlay();

	CameraComponent = GetComponentByClass<UCameraComponent>();
	PlayerControllerRef = Cast<AProjectLiminalPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	Torchlight->SetVisibility(false);

	StartingCameraLocation = CameraComponent->GetComponentLocation();
	StartingCameraRotation = CameraComponent->GetComponentRotation();
}

void APeepHole::Tick(float DeltaTime)
{
	// Only ticks if player is zooming into hole
	if (PeepHoleState == EPS_ZoomingIn && InteractCamSpringArm)
	{
		float CurrentSpringArmLength = InteractCamSpringArm->TargetArmLength;
		float NewSpringArmLocation = FMath::FInterpTo(CurrentSpringArmLength, ZoomedInSpringArmLength, DeltaTime, ZoomSpeed);
		InteractCamSpringArm->TargetArmLength = NewSpringArmLocation;

		if (InteractCamSpringArm->TargetArmLength == ZoomedInSpringArmLength)
		{
			PeepHoleState = EPS_Active;
		}
	}

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

	PeepHoleState = EPS_ZoomingIn;
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
	Super::ReturnPlayerToFloor(Player);

	PeepHoleState = EPS_NotActive;

	// Reset components ready for another use
	Torchlight->SetVisibility(false);
	InteractCamSpringArm->TargetArmLength = StartingSpringArmLength;
}
