// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Codes/PressableButton.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Items/InteractableBase.h"
#include "Items/Codes/CodeComponent.h"

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
}

// Called when the game starts or when spawned
void APressableButton::BeginPlay()
{
	Super::BeginPlay();
	
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
		CodeComponent->EnterDigitToCode(ButtonArrayValue);
		UE_LOG(LogTemp, Warning, TEXT("%s Pressed!"), *GetName());
	}
}

