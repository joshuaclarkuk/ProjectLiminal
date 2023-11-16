// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"
#include "Characters/ProjectLiminalCharacter.h"
#include "Config/ProjectLiminalPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"

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
	PlayerCamera = PlayerCharacter->GetFirstPersonCameraComponent();
}

void UInventoryComponent::AddItemToInventory(AActor* Item)
{
	if (Item)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item Added to Inventory: %s"), *Item->GetName());
		Items.Add(Item);
	}
}

void UInventoryComponent::DisplayInventory()
{
	if (PlayerCharacter && PlayerCamera)
	{
		PlayerCharacter->SetPlayerState(EPS_InInventory);
		AlterCameraDepthOfField(true);

		for (int i = 0; i < Items.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("Inventory contains: %s"), *Items[i]->GetName());
		}
	}
}

void UInventoryComponent::CloseInventory()
{
	if (PlayerCharacter && PlayerCamera)
	{
		PlayerCharacter->SetPlayerState(EPS_Unoccupied);
		AlterCameraDepthOfField(false);
		UE_LOG(LogTemp, Warning, TEXT("Inventory closed"));
	}
}

void UInventoryComponent::AlterCameraDepthOfField(bool Enable)
{
	PlayerCamera->PostProcessSettings.bOverride_DepthOfFieldScale = Enable;
	PlayerCamera->PostProcessSettings.DepthOfFieldScale = Enable ? 1.0f : 0.0f;
}

