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

}

void APressableButton::TriggerButton(int32 ButtonArrayValue)
{
	if (CodeComponent)
	{
		DepressButton();
		GetWorldTimerManager().SetTimer(ButtonPushHandle, this, &APressableButton::ReturnButtonToOriginalPosition, ButtonPushDuration, false);
		CodeComponent->EnterDigitToCode(ButtonArrayValue);
		UGameplayStatics::PlaySoundAtLocation(this, SoundEffect, GetActorLocation());
		UE_LOG(LogTemp, Warning, TEXT("%s Pressed!"), *GetName());
	}
}

void APressableButton::DepressButton()
{
	SetActorLocation(ButtonStartPosition + FVector(0.0f, 0.0f, -10.0f));
	SetActorRotation(ButtonStartRotation + FRotator(-10.0f, 0.0f, 0.0f));
}

void APressableButton::ReturnButtonToOriginalPosition()
{
	SetActorLocation(ButtonStartPosition);
	SetActorRotation(ButtonStartRotation);
}

