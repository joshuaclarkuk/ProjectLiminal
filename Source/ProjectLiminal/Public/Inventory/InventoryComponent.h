// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AProjectLiminalCharacter;
class AProjectLiminalPlayerController;
class APlayerHUD;
class UOverlayWidget;
class UCameraComponent;
class AItemBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTLIMINAL_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	float ItemSpacing = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FRotator RotationToAddToInventoryItem = FRotator(0.0f, 50.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	float ScrollSpeed = 600.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FVector> ItemStartingPositions;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FVector> ItemTargetPositions;

	AProjectLiminalCharacter* PlayerCharacter;
	AProjectLiminalPlayerController* LiminalPlayerController;
	APlayerHUD* PlayerHUD;
	UOverlayWidget* OverlayWidget;
	UCameraComponent* PlayerCamera;

	int32 ScrollIndex = 0;
	bool bShouldScroll = false;
	bool bIsScrollingLeft = true;
	bool bIsScrolling = false;

	void DisplayItems();
	void ScrollThroughItems(float DeltaTime);
	void DetermineValidScroll();

public:
	void AddItemToInventory(AItemBase* Item);
	void OpenInventory();
	void CloseInventory();
	void SetScrollBehaviour(bool ShouldStartScrolling, bool ShouldScrollLeft);
	bool GetIsScrolling() { return bIsScrolling; }
	TArray<AItemBase*> GetInventoryList() { return Items; }
};
