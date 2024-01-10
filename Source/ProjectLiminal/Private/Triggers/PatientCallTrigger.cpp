// Fill out your copyright notice in the Description page of Project Settings.


#include "Triggers/PatientCallTrigger.h"
#include "Components/BoxComponent.h"
#include "Characters/OtherPatient.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstance.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APatientCallTrigger::APatientCallTrigger()
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
void APatientCallTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	DrawDebugBox(GetWorld(), GetActorLocation(), FVector(30.0f, 30.0f, 30.0f), FColor::Green, false, 10.0f);

	// Assign reference to screenmeshcomponent
	ScreenMeshComponent = DisplayScreenActor->GetComponentByClass<UStaticMeshComponent>();

	// Bind function to overlap delegate
	BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &APatientCallTrigger::OnTriggerOverlap);
}

// Called every frame
void APatientCallTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APatientCallTrigger::OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bHasBeenTriggered) { return; }

	// Trigger Patient navigation code
	if (OtherPatientToCall)
	{
		UE_LOG(LogTemp, Display, TEXT("Navigation triggered"));
		OtherPatientToCall->PauseAndInitiateMovement();
		bHasBeenTriggered = true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No OtherPatientToCall assigned to %s"), *GetActorNameOrLabel());
	}

	// Trigger Screen display code
	if (DisplayScreenActor)
	{
		if (ScreenMeshComponent)
		{
			if (PatientDoorCodeDisplay)
			{
				ScreenMeshComponent->SetMaterial(0, PatientDoorCodeDisplay);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("No patient door code material instance set on %s"), *GetActorNameOrLabel());
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ScreenMeshComponent not found on DisplayScreenActor via %s. Reassign on trigger volume"), *GetActorNameOrLabel());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No screen actor reference found on %s"), *GetActorNameOrLabel());
	}

	// Trigger Noise
	if (PatientCallSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PatientCallSound, DisplayScreenActor->GetActorLocation());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No PatientCallSound entered on %s"), *GetActorNameOrLabel());
	}
}

