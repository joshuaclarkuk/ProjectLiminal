// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/StartMenuDummyPawn.h"
#include "Camera/CameraComponent.h"

// Sets default values
AStartMenuDummyPawn::AStartMenuDummyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DummyCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("DummyCameraComponent"));
	DummyCameraComponent->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AStartMenuDummyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStartMenuDummyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
