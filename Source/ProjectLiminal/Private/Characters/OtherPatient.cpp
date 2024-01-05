// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/OtherPatient.h"

// Sets default values
AOtherPatient::AOtherPatient()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AOtherPatient::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOtherPatient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOtherPatient::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

