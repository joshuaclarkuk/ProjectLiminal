// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"

// Sets default values
AInteractable::AInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(GetRootComponent());
	BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollider->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	BoxCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	BoxCollider->SetNotifyRigidBodyCollision(true);
	BoxCollider->SetMobility(EComponentMobility::Stationary);
	SetRootComponent(BoxCollider);

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	ObjectMesh->SetupAttachment(BoxCollider);
	ObjectMesh->SetMobility(EComponentMobility::Stationary);
	ObjectMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	InteractPrompt = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractPrompt"));
	InteractPrompt->SetupAttachment(ObjectMesh);
	InteractPrompt->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	
	InteractPrompt->SetVisibility(false);
}

// Called every frame
void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractable::SetInteractPromptVisibility(bool bIsVisible)
{
	InteractPrompt->SetVisibility(bIsVisible);
}