// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Items/ItemBase.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	TicketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TicketMesh"));
	TicketMesh->SetupAttachment(GetRootComponent());
	TicketMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemBase::ToggleVisibilityInGame(bool IsVisible)
{
	SetActorHiddenInGame(!IsVisible);
	SetActorTickEnabled(!IsVisible);
}


