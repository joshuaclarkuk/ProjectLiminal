// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/InteractableBase.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/ProjectLiminalCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Config/ProjectLiminalPlayerController.h"
#include "Blueprint/WidgetLayoutLibrary.h"


// Sets default values
AInteractableBase::AInteractableBase()
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
	InteractPrompt->SetWorldScale3D(FVector(0.25f, 0.25f, 0.25f));
	InteractPrompt->SetDrawSize(FVector2D(1280.0f, 256.0f));
	InteractPrompt->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InteractPrompt->SetAutoActivate(false);

	InteractCamSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	InteractCamSpringArm->SetupAttachment(BoxCollider);
	InteractCamSpringArm->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	InteractCamSpringArm->bDoCollisionTest = false;

	InteractCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("InteractCamera"));
	InteractCamera->SetupAttachment(InteractCamSpringArm);

	CameraLockPosition = CreateDefaultSubobject<USceneComponent>(TEXT("CameraLockPosition"));
	CameraLockPosition->SetupAttachment(BoxCollider);
}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	
	InteractPrompt->SetVisibility(false);
}

void AInteractableBase::SetInteractPromptVisibility(bool bIsVisible)
{
	InteractPrompt->SetVisibility(bIsVisible);
}

void AInteractableBase::MovePlayerInFrontOfObject()
{	
	AProjectLiminalPlayerController* PlayerController = Cast<AProjectLiminalPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{		
		PlayerController->ZoomOnObjectAndEnableMouse(this, CameraSnapSpeedInSeconds, false);
	}
}

void AInteractableBase::ReturnPlayerToFloor(AProjectLiminalCharacter* Player)
{
	AProjectLiminalPlayerController* PlayerController = Cast<AProjectLiminalPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->ZoomBackOutAndDisableMouse(Player, CameraSnapSpeedInSeconds);
	}
}