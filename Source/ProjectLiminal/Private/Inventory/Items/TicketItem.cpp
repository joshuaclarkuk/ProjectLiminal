// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Items/TicketItem.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATicketItem::ATicketItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TicketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TicketMesh"));
	TicketMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void ATicketItem::BeginPlay()
{
	Super::BeginPlay();
	
}


