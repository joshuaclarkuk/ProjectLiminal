// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Codes/PressableButton.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Items/InteractableBase.h"
#include "Items/Codes/CodeComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APressableButton::APressableButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	SetRootComponent(BoxCollider);

	ButtonMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMeshComponent"));
	ButtonMeshComponent->SetupAttachment(BoxCollider);
	ButtonMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SoundEffect = CreateDefaultSubobject<USoundBase>(TEXT("ButtonPressSFX"));
}

// Called when the game starts or when spawned
void APressableButton::BeginPlay()
{
	Super::BeginPlay();

	// Set up button push mechanics
	ButtonStartPosition = GetActorLocation();
	ButtonStartRotation = GetActorRotation();
	
	if (InteractableObject)
	{
		CodeComponent = InteractableObject->GetComponentByClass<UCodeComponent>();
	}
}

// Called every frame
void APressableButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsBeingDepressed)
	{
		DepressButton(DeltaTime);
	}
	else if (bIsRising)
	{
		ReturnButtonToOriginalPosition(DeltaTime);
	}
}

void APressableButton::TriggerButton(int32 ButtonArrayValue)
{
	if (CodeComponent && !bIsBeingDepressed && !bIsRising)
	{
		bIsBeingDepressed = true;
		CodeComponent->EnterDigitToCode(ButtonArrayValue);
		if (SoundEffect)
		{
			UGameplayStatics::PlaySoundAtLocation(this, SoundEffect, GetActorLocation());
		}
	}
}

void APressableButton::DepressButton(float DeltaTime)
{
	FVector TargetLocation = ButtonStartPosition + FVector(0.0f, 0.0f, -2.0f);
	FRotator TargetRotation = ButtonStartRotation + FRotator(-10.0f, 0.0f, 0.0f);

	FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, ButtonPushInterpSpeed);
	FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, ButtonPushInterpSpeed);
	SetActorLocation(NewLocation);
	SetActorRotation(NewRotation);
	
	// Disable
	if (NewLocation.Equals(TargetLocation, 0.01f) && NewRotation.Equals(TargetRotation, 0.1f))
	{
		bIsBeingDepressed = false;
		bIsRising = true;
	}
}

void APressableButton::ReturnButtonToOriginalPosition(float DeltaTime)
{
	FVector NewLocation = FMath::VInterpTo(GetActorLocation(), ButtonStartPosition, DeltaTime, ButtonPushInterpSpeed);
	FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), ButtonStartRotation, DeltaTime, ButtonPushInterpSpeed);
	SetActorLocation(NewLocation);
	SetActorRotation(NewRotation);

	if (NewLocation.Equals(ButtonStartPosition, 0.01f) && NewRotation.Equals(ButtonStartRotation, 0.1f))
	{
		bIsRising = false;
	}
}