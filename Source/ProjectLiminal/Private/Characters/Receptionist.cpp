// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Receptionist.h"
#include "NiagaraComponent.h"

// Sets default values
AReceptionist::AReceptionist()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ReceptionistSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ReceptionistSceneComponent"));
	SetRootComponent(ReceptionistSceneComponent);

	ReceptionistBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ReceptionistBodyMesh"));
	ReceptionistBodyMesh->SetupAttachment(ReceptionistSceneComponent);
	ReceptionistBodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BlackSmokeEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BlackSmokeEffect"));
	BlackSmokeEffect->SetupAttachment(ReceptionistBodyMesh);
}

// Called when the game starts or when spawned
void AReceptionist::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AReceptionist::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

