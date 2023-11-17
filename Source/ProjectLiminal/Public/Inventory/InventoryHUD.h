// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InventoryHUD.generated.h"

class UInventoryOverlay;

/**
 * 
 */
UCLASS()
class PROJECTLIMINAL_API AInventoryHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Project Liminal Config")
	TSubclassOf<UInventoryOverlay> InventoryOverlayClass;

	UInventoryOverlay* InventoryOverlayRef;

	/*GETTERS & SETTERS*/
public:
	UInventoryOverlay* GetInventoryOverlay() { return InventoryOverlayRef; }
};
