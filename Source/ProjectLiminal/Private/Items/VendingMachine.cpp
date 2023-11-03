// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/VendingMachine.h"

AVendingMachine::AVendingMachine()
{

}

void AVendingMachine::SetInteractPromptVisibility(bool bVisible)
{
	Super::SetInteractPromptVisibility(bVisible);
}

void AVendingMachine::MovePlayerInFrontOfObject()
{
	Super::MovePlayerInFrontOfObject();
}

void AVendingMachine::ReturnPlayerToFloor(AProjectLiminalCharacter* Player)
{
	Super::ReturnPlayerToFloor(Player);
}
