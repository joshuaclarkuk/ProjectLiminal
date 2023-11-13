// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/InteractableBase.h"
#include "VendingMachine.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLIMINAL_API AVendingMachine : public AInteractableBase
{
	GENERATED_BODY()
	
public:
	AVendingMachine();

protected:
	virtual void MovePlayerInFrontOfObject() override;
};
