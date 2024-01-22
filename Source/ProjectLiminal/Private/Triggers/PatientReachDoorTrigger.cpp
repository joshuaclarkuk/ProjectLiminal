// Fill out your copyright notice in the Description page of Project Settings.


#include "Triggers/PatientReachDoorTrigger.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/MoveWithInterpComponent.h"

// Sets default values
APatientReachDoorTrigger::APatientReachDoorTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	SetRootComponent(BoxTrigger);
	BoxTrigger->SetCollisionProfileName("Trigger");
	BoxTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxTrigger->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void APatientReachDoorTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	ScreenMeshComponent = DisplayScreenActor->GetComponentByClass<UStaticMeshComponent>();

	BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &APatientReachDoorTrigger::OnTriggerOverlap);

}

// Called every frame
void APatientReachDoorTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APatientReachDoorTrigger::OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GetWorldTimerManager().SetTimer(DelayForEffectTimerHandle, this, &APatientReachDoorTrigger::OpenDoorAfterDelay, 4.0f, false);
}

void APatientReachDoorTrigger::OpenDoorAfterDelay()
{
	PlaySuccessSound();
	SetDefaultDisplayScreenMaterial();
	OpenDoor();
}

void APatientReachDoorTrigger::PlaySuccessSound()
{
	if (PatientCodeSuccessSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PatientCodeSuccessSound, GetActorLocation());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No PatientSuccessSound assigned to %s"), *GetActorNameOrLabel());
	}
}

void APatientReachDoorTrigger::SetDefaultDisplayScreenMaterial()
{
	if (DisplayScreenDefaultMaterial)
	{
		if (ScreenMeshComponent)
		{
			ScreenMeshComponent->SetMaterial(0, DisplayScreenDefaultMaterial);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No ScreenMeshComponent found on %s, setting to null"), *GetActorNameOrLabel());
			ScreenMeshComponent->SetMaterial(0, NULL);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No default material instance for DisplayScreen set in %s"), *GetActorNameOrLabel());
	}
}

void APatientReachDoorTrigger::OpenDoor()
{
	if (DoorToOpen)
	{
		UMoveWithInterpComponent* MoveWithInterpComponent = DoorToOpen->GetComponentByClass<UMoveWithInterpComponent>();

		if (MoveWithInterpComponent)
		{
			MoveWithInterpComponent->SetShouldRotate(true);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No MoveWithInterpComponent found on Door, check referenced door on %s has correct component"), *GetActorNameOrLabel());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No door to open assigned to %s, drag in new reference"), *GetActorNameOrLabel());
	}
}

