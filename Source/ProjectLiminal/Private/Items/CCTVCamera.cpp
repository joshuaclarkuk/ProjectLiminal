// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/CCTVCamera.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"


// Sets default values
ACCTVCamera::ACCTVCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(GetRootComponent());

	CameraHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CameraHead"));
	CameraHead->SetupAttachment(CameraArm);

	CameraLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("CameraLight"));
	CameraLight->SetupAttachment(CameraHead);
	CameraLight->SetRelativeLocation(FVector(26.03f, -8.33f, 24.53f));
	CameraLight->SetAttenuationRadius(5.0f);
	CameraLight->SetLightColor(FLinearColor::Green);

    TracePoint = CreateDefaultSubobject<USceneComponent>(TEXT("TracePoint"));
    TracePoint->SetupAttachment(CameraHead);
    TracePoint->SetRelativeLocation(FVector(40.0f, 0.0f, 30.0f));
}

// Called when the game starts or when spawned
void ACCTVCamera::BeginPlay()
{
	Super::BeginPlay();

	// Allow camera to find player
	PlayerActor = UGameplayStatics::GetPlayerPawn(this, 0);

	// Start trace for player at given interval
	GetWorldTimerManager().SetTimer(TraceTimerHandle, this, &ACCTVCamera::TraceForPlayer, TraceInterval, true);
}

// Called every frame
void ACCTVCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerActor && CameraHead)
	{
		RotateToPlayer(PlayerActor->GetActorLocation());
	}
}

void ACCTVCamera::RotateToPlayer(FVector PlayerLocation)
{
	// Vector maths to determine the Vector from the turret to the target
	FVector ToTarget = PlayerLocation - CameraHead->GetComponentLocation();

	// Zeroing pitch and roll and using the Yaw component of FVector function .Rotation() to turn turret while keeping it parallel to ground
	FRotator LookAtRotation = FRotator(FMath::ClampAngle(ToTarget.Rotation().Pitch, -30.0f, 0.0f), FMath::ClampAngle(ToTarget.Rotation().Yaw,-180.0f, 0.0f), 0.0f);

	CameraHead->SetWorldRotation(FMath::RInterpTo(CameraHead->GetComponentRotation(), LookAtRotation, GetWorld()->GetDeltaSeconds(), 10.0f));
}

void ACCTVCamera::TraceForPlayer()
{
    if (PlayerIsInSight())
    {
        CameraLight->SetLightColor(FLinearColor::Green);

        if (!bPlayerPromptedToAlert && PlayerSightedSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, PlayerSightedSound, GetActorLocation());
            bPlayerPromptedToAlert = true;
        }
    }
    else
    {
        CameraLight->SetLightColor(FLinearColor::Red);
        bPlayerPromptedToAlert = false;
    }
}

bool ACCTVCamera::PlayerIsInSight()
{
    if (!TracePoint || !PlayerActor)
    {
        // Object or Player reference is invalid
        return false;
    }

    FCollisionQueryParams TraceParams(FName(TEXT("LineOfSight")), true, this);
    TraceParams.bTraceComplex = false; // Enable detailed collision detection
    TraceParams.bReturnPhysicalMaterial = false;

    FHitResult HitResult;
    FVector ObjectLocation = TracePoint->GetComponentLocation();
    FVector ForwardVector = TracePoint->GetForwardVector();
    FVector TraceEnd = ObjectLocation + ForwardVector * TraceDistance;

    // Perform line trace
    GetWorld()->LineTraceSingleByChannel(
        HitResult,
        ObjectLocation,
        TraceEnd,
        ECC_Pawn,
        TraceParams
    );

    if (HitResult.GetActor() == PlayerActor)
    {
        // Player is in sight
        return true;
    }

    // Player is not in sight
    return false;
}

