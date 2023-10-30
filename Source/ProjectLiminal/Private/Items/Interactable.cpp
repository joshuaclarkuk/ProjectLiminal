// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Interactable.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/ProjectLiminalCharacter.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include "GameFramework/CharacterMovementComponent.h"

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

	CameraLockPosition = CreateDefaultSubobject<USceneComponent>(TEXT("CameraLockPosition"));
	CameraLockPosition->SetupAttachment(BoxCollider);
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

void AInteractable::MovePlayerInFrontOfObject(UCameraComponent* PlayerCamera, AProjectLiminalCharacter* Player)
{	
	// Capture original position so it can return here
	OriginalPlayerPosition = Player->GetActorLocation();

	// Set player movement before disabling gravity
	// Eliminates drift while looking at object
	UCharacterMovementComponent* PlayerMovement = Player->GetComponentByClass<UCharacterMovementComponent>();
	if (PlayerMovement)
	{
		PlayerMovement->Velocity = FVector::Zero();
	}

	// Disable gravity to prevent playing falling to ground, then move player in front of object
	Player->GetCharacterMovement()->GravityScale = 0;
	Player->SetActorLocation(CameraLockPosition->GetComponentLocation());
	
	// Rotate player camera to face object
	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);	
	if (Controller)
	{
		FRotator NewCameraRotation = UKismetMathLibrary::FindLookAtRotation(PlayerCamera->GetComponentLocation(), this->GetActorLocation());
		Controller->SetControlRotation(NewCameraRotation);
	}
}

void AInteractable::ReturnPlayerToFloor(UCameraComponent* PlayerCamera, AProjectLiminalCharacter* Player)
{
	Player->SetActorLocation(OriginalPlayerPosition);
	Player->GetCharacterMovement()->GravityScale = 1;
}
