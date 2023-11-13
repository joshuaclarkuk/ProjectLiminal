// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/VendingMachine.h"
#include "Config/ProjectLiminalPlayerController.h"


AVendingMachine::AVendingMachine()
{

}

// Overriding this method from InteractableBase, as Vending Machine doesn't actually require mouse.
void AVendingMachine::MovePlayerInFrontOfObject()
{
	Super::MovePlayerInFrontOfObject();

	AProjectLiminalPlayerController* PlayerController = Cast<AProjectLiminalPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->SetShowMouseCursor(false);
	}
}

