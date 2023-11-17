// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryHUD.h"
#include "Inventory/InventoryOverlay.h"
#include "Kismet/GameplayStatics.h"
#include "Config/ProjectLiminalPlayerController.h"

void AInventoryHUD::BeginPlay()
{
	Super::BeginPlay();

	AProjectLiminalPlayerController* PlayerController = Cast<AProjectLiminalPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController && InventoryOverlayClass)
	{
		UInventoryOverlay* InventoryOverlay = CreateWidget<UInventoryOverlay>(PlayerController, InventoryOverlayClass);
		InventoryOverlay->AddToViewport();
		InventoryOverlayRef = InventoryOverlay;
	}
}