// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"
#include "Characters/ProjectLiminalCharacter.h"
#include "Config/ProjectLiminalPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Inventory/InventoryHUD.h"
#include "Inventory/InventoryOverlay.h"
#include "Camera/CameraComponent.h"
#include "Inventory/Items/ItemBase.h"
#include "Inventory/Items/Item_Ticket.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<AProjectLiminalCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	LiminalPlayerController = Cast<AProjectLiminalPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	InventoryHUD = Cast<AInventoryHUD>(LiminalPlayerController->GetHUD());
	InventoryOverlay = InventoryHUD->GetInventoryOverlay();
	PlayerCamera = PlayerCharacter->GetComponentByClass<UCameraComponent>();
}

void UInventoryComponent::AddItemToInventory(AItemBase* Item)
{
	if (Item)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item Added to Inventory: %s"), *Item->GetName());
		Items.Add(Item);
	}
}

void UInventoryComponent::DisplayInventory()
{
	if (PlayerCharacter && InventoryOverlay && PlayerCamera)
	{
		PlayerCharacter->SetPlayerState(EPS_InInventory);
		PlayerCamera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
		PlayerCamera->PostProcessSettings.DepthOfFieldFocalDistance = AdjustedDepthOfField;
		DisplayInventoryItem();
		//InventoryOverlay->SetBlurState(EBlurState::EBS_Blurring);
		for (int i = 0; i < Items.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("Inventory contains: %s"), *Items[i]->GetName());
		}
	}
}

void UInventoryComponent::CloseInventory()
{
	if (PlayerCharacter && InventoryOverlay && PlayerCamera)
	{
		PlayerCharacter->SetPlayerState(EPS_Unoccupied);
		PlayerCamera->PostProcessSettings.DepthOfFieldFocalDistance = DefaultDepthOfField;
		PlayerCamera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = false;
		//InventoryOverlay->SetBlurState(EBlurState::EBS_Deblurring);
		UE_LOG(LogTemp, Warning, TEXT("Inventory closed"));
	}
}

void UInventoryComponent::DisplayInventoryItem()
{
	if (!Items.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Location to move object to is: %s"), *PlayerCharacter->GetLocationToDisplayInventoryItem().ToString());
		Items[0]->SetActorLocation(PlayerCharacter->GetLocationToDisplayInventoryItem());
		Items[0]->SetActorRotation(PlayerCharacter->GetRotationToDisplayInventoryItem());
		Items[0]->ToggleVisibilityInGame(true);
	}
}

