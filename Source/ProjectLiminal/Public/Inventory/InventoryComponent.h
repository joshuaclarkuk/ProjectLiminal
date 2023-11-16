// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AProjectLiminalCharacter;
class AProjectLiminalPlayerController;
class UCameraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTLIMINAL_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 MaxItemsInInventory = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<AActor*> Items;

	AProjectLiminalCharacter* PlayerCharacter;
	AProjectLiminalPlayerController* LiminalPlayerController;
	UCameraComponent* PlayerCamera;

	void AlterCameraDepthOfField(bool Enable);

public:
	void AddItemToInventory(AActor* Item);
	void DisplayInventory();
	void CloseInventory();
};
