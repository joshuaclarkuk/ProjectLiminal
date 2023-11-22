// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"
#include "Characters/ProjectLiminalCharacter.h"
#include "Config/ProjectLiminalPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Inventory/Items/ItemBase.h"
#include "Inventory/Items/Item_Ticket.h"
#include "Components/StaticMeshComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<AProjectLiminalCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	LiminalPlayerController = Cast<AProjectLiminalPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	PlayerCamera = PlayerCharacter->GetComponentByClass<UCameraComponent>();
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// Tick function disabled by default
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Rotate objects around Z axis when displaying inventory
	if (!Items.IsEmpty())
	{
		for (int i = 0; i < Items.Num(); i++)
		{
			FRotator CurrentRotation = Items[i]->GetActorRotation();
			FRotator RotationToAdd = RotationToAddToInventoryItem;
			Items[i]->SetActorRotation(CurrentRotation + RotationToAdd * DeltaTime);
		}

		if (bShouldScroll && !ItemStartingPositions.IsEmpty() && !ItemTargetPositions.IsEmpty())
		{
			ScrollThroughItems(DeltaTime);
		}
	}
}

void UInventoryComponent::AddItemToInventory(AItemBase* Item)
{
	if (Item)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item Added to Inventory: %s"), *Item->GetName());
		Item->GetComponentByClass<UStaticMeshComponent>()->CastShadow = false;
		Items.Add(Item);
	}
}

void UInventoryComponent::OpenInventory()
{
	if (PlayerCharacter && PlayerCamera &&!Items.IsEmpty())
	{
		PlayerCharacter->SetPlayerState(EPS_InInventory);
		SetComponentTickEnabled(true);
		PlayerCamera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
		PlayerCamera->PostProcessSettings.DepthOfFieldFocalDistance = AdjustedDepthOfField;
		DisplayItems();
	}
}

void UInventoryComponent::CloseInventory()
{
	if (PlayerCharacter && PlayerCamera && !bIsScrolling)
	{
		// Lock player and perform DOF effect
		PlayerCharacter->SetPlayerState(EPS_Unoccupied);
		SetComponentTickEnabled(false);
		PlayerCamera->PostProcessSettings.DepthOfFieldFocalDistance = DefaultDepthOfField;
		PlayerCamera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = false;

		// Hide Inventory items on closure
		for (int i = 0; i < Items.Num(); i++)
		{
			Items[i]->ToggleVisibilityInGame(false);
		}

		// Reset arrays and ScrollIndex ready for next Inventory screen
		ScrollIndex = 0;
		ItemStartingPositions.Empty();
		ItemTargetPositions.Empty();
	}
}

void UInventoryComponent::DisplayItems()
{ 
	for (int i = 0; i < Items.Num(); i++)
	{
		Items[i]->SetActorLocation(PlayerCharacter->GetLocationToDisplayInventoryItem() + PlayerCharacter->GetActorRightVector() * ItemSpacing * i);
		Items[i]->SetActorRotation(PlayerCharacter->GetRotationToDisplayInventoryItem());
		Items[i]->ToggleVisibilityInGame(true);
	}
}

void UInventoryComponent::ScrollThroughItems(float DeltaTime)
{
	bIsScrolling = true;
	for (int i = 0; i < Items.Num(); i++)
	{
		FVector CurrentPosition = Items[i]->GetActorLocation();
		FVector NewPosition = FMath::VInterpConstantTo(CurrentPosition, ItemTargetPositions[i], DeltaTime, ScrollSpeed);

		Items[i]->SetActorLocation(NewPosition);

		if (CurrentPosition.Equals(ItemTargetPositions[i], 1.0f))
		{
			bShouldScroll = false;
			bIsScrolling = false;
		}
	}
}

void UInventoryComponent::SetScrollBehaviour(bool ShouldStartScrolling, bool ShouldScrollLeft)
{
	bIsScrollingLeft = ShouldScrollLeft;

	// Re-initialise arrays
	ItemStartingPositions.Empty();
	ItemTargetPositions.Empty();

	DetermineValidScroll();

	// Trigger scrolling
	bShouldScroll = ShouldStartScrolling;
}

void UInventoryComponent::DetermineValidScroll()
{
	// Check there is an item to scroll to
	if (bIsScrollingLeft && ScrollIndex < Items.Num() - 1)
	{
		// Initialise starting positions
		for (int i = 0; i < Items.Num(); i++)
		{
			ItemStartingPositions.Add(Items[i]->GetActorLocation());
			ItemTargetPositions.Add(ItemStartingPositions[i] - PlayerCharacter->LocationToDisplayInventoryItem->GetRightVector() * ItemSpacing);
		}

		ScrollIndex++;
	}
	// Check there is an item to scroll to
	else if (!bIsScrollingLeft && ScrollIndex > 0)
	{
		// Initialise starting positions
		for (int i = 0; i < Items.Num(); i++)
		{
			ItemStartingPositions.Add(Items[i]->GetActorLocation());
			ItemTargetPositions.Add(ItemStartingPositions[i] + PlayerCharacter->LocationToDisplayInventoryItem->GetRightVector() * ItemSpacing);
		}

		ScrollIndex--;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot scroll. ScrollIndex out of bounds."));
	}
}

