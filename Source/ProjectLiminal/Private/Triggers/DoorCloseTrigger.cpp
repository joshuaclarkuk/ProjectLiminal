// Fill out your copyright notice in the Description page of Project Settings.


#include "Triggers/DoorCloseTrigger.h"
#include "Components/BoxComponent.h"
#include "Components/MoveWithInterpComponent.h"

// Sets default values
ADoorCloseTrigger::ADoorCloseTrigger()
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
void ADoorCloseTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADoorCloseTrigger::OnTriggerOverlap);
}

void ADoorCloseTrigger::OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (DoorToClose)
	{
		UMoveWithInterpComponent* MoveWithInterpComponent = DoorToClose->GetComponentByClass<UMoveWithInterpComponent>();
		if (MoveWithInterpComponent)
		{
			MoveWithInterpComponent->SetShouldRotate(false);
			MoveWithInterpComponent->SetShouldRotateBack(true);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No MoveWithInterpComponent found on Door assigned to %s"), *GetActorNameOrLabel());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No DoorToClose assigned to %s"), * GetActorNameOrLabel());
	}
}

// Called every frame
void ADoorCloseTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

