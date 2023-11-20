// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AProjectLiminalCharacter;
class AProjectLiminalPlayerController;
class UCameraComponent;
class AItemBase;

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
	TArray<AItemBase*> Items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	float AdjustedDepthOfField = 20.0f;
	float DefaultDepthOfField = 0.0f;

	AProjectLiminalCharacter* PlayerCharacter;
	AProjectLiminalPlayerController* LiminalPlayerController;
	UCameraComponent* PlayerCamera;

	void DisplayInventoryItem();

public:
	void AddItemToInventory(AItemBase* Item);
	void DisplayInventory();
	void CloseInventory();
};
