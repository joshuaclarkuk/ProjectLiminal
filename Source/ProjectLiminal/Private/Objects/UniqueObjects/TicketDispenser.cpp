// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/UniqueObjects/TicketDispenser.h"
#include "Components/SpotLightComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Config/ProjectLiminalPlayerController.h"
#include "Characters/ProjectLiminalCharacter.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/Items/ItemBase.h"

ATicketDispenser::ATicketDispenser()
{
	SmallOrangeLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SmallOrangeLight"));
	SmallOrangeLight->SetupAttachment(GetRootComponent());
	SmallOrangeLight->SetRelativeLocationAndRotation(FVector(0.886f, 0.0f, 34.5f),FRotator(-50.0f, 0.0f, 0.0f));
	SmallOrangeLight->SetIntensity(300.0f);
	SmallOrangeLight->SetAttenuationRadius(100.0f);
	SmallOrangeLight->SetInnerConeAngle(26.2f);
	SmallOrangeLight->SetOuterConeAngle(80.0f);
	SmallOrangeLight->bUseTemperature = true;
	SmallOrangeLight->SetTemperature(1600.0f);
}

void ATicketDispenser::BeginPlay()
{
	Super::BeginPlay();

	// Initialise Player Controller reference
	LiminalPlayerController = Cast<AProjectLiminalPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	// Initialise Player Character reference
	PlayerCharacter = Cast<AProjectLiminalCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	// Initialise Player Inventory reference
	if (PlayerCharacter)
	{
		PlayerInventory = PlayerCharacter->GetComponentByClass<UInventoryComponent>();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter not found by %s"), *GetActorNameOrLabel());
	}

	// Set timer to flicker light
	GetWorldTimerManager().SetTimer(LightFadeTimerHandle, this, &ATicketDispenser::MakeLightFlicker, FMath::FRandRange(0.1f, 0.4f), true);

	SmallOrangeLight->SetVisibility(false);	
	SmallOrangeLight->SetIntensity(MaxLightIntensity);
}

void ATicketDispenser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SmallOrangeLight && bShouldTurnOn)
	{
		MakeLightFade(DeltaTime);
	}
}

void ATicketDispenser::MakeLightFade(float DeltaTime)
{
	float CurrentIntensity = SmallOrangeLight->Intensity;	
	float NewIntensity = FMath::FInterpConstantTo(CurrentIntensity, TargetLightIntensity, DeltaTime, LightInterpSpeed);
	SmallOrangeLight->SetIntensity(NewIntensity);
}

void ATicketDispenser::MakeLightFlicker()
{
	if (bShouldTurnOn)
	{
		if (bShouldFlickerOn)
		{
			SmallOrangeLight->SetVisibility(true);
			bShouldFlickerOn = false;
		}
		else
		{
			SmallOrangeLight->SetVisibility(false);
			bShouldFlickerOn = true;
		}

		FlickerInterval = FMath::FRandRange(0.1f, 1.0f);
		TargetLightIntensity = FMath::FRandRange(MaxLightIntensity / 2, MaxLightIntensity);
		GetWorldTimerManager().SetTimer(LightFadeTimerHandle, this, &ATicketDispenser::MakeLightFlicker, FlickerInterval, true);
	}
}

void ATicketDispenser::MovePlayerInFrontOfObject()
{
	Super::MovePlayerInFrontOfObject();

	if (PlayerCharacter)
	{
		// Disable input while animation plays (not currently in use)
		PlayerCharacter->DisableInput(LiminalPlayerController);

		if (PlayerInventory && TicketItem)
		{
			// TODO: Play ticket grab animation
			PlayerInventory->AddItemToInventory(TicketItem);
			TicketItem->ToggleVisibilityInGame(false);
			bHasBeenSolved = true;
		}
		PlayerCharacter->EnableInput(LiminalPlayerController);
	}
}
