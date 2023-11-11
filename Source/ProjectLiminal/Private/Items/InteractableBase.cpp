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
#include "Items/Codes/CodeComponent.h"
#include "Items/Codes/PressableButton.h"
#include "Components/PointLightComponent.h"

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
	InteractPrompt->SetDrawSize(FVector2D(2500.0f, 500.0f));
	InteractPrompt->SetAutoActivate(false);

	InteractCamSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	InteractCamSpringArm->SetupAttachment(BoxCollider);
	InteractCamSpringArm->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	InteractCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("InteractCamera"));
	InteractCamera->SetupAttachment(InteractCamSpringArm);

	CameraLockPosition = CreateDefaultSubobject<USceneComponent>(TEXT("CameraLockPosition"));
	CameraLockPosition->SetupAttachment(BoxCollider);

	CodeComponent = CreateDefaultSubobject<UCodeComponent>(TEXT("CodeComponent"));

	CodeIndicatorLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("CodeIndicatorComponent"));
	CodeIndicatorLight->SetupAttachment(ObjectMesh);
	CodeIndicatorLight->SetLightColor(FLinearColor::Red);
	CodeIndicatorLight->SetAttenuationRadius(3.0f);
	CodeIndicatorLight->SetIntensity(5000.0f);
}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	
	InteractPrompt->SetVisibility(false);

	ConstructPressableButtonArray();
}

// Called every frame
void AInteractableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		PlayerController->ZoomOnObjectAndEnableMouse(this, CameraSnapSpeedInSeconds);
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

void AInteractableBase::ConstructPressableButtonArray()
{
	// Get the array of child actors attached to this actor
	GetAttachedActors(ArrayOfAttachedButtons);
}

void AInteractableBase::PressButton(int32 ButtonArrayValue)
{
	if (ArrayOfAttachedButtons[ButtonArrayValue])
	{
		APressableButton* PressableButton = CastChecked<APressableButton>(ArrayOfAttachedButtons[ButtonArrayValue]);
		if (PressableButton)
		{
			PressableButton->TriggerButton(ButtonArrayValue);
		}
	}
}
