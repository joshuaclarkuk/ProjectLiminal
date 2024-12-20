// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/InteractableBase.h"
#include "TicketDispenser.generated.h"

class USpotLightComponent;
class AProjectLiminalPlayerController;
class AProjectLiminalCharacter;
class UInventoryComponent;
class AItemBase;

/**
 * 
 */
UCLASS()
class PROJECTLIMINAL_API ATicketDispenser : public AInteractableBase
{
	GENERATED_BODY()

public:
	ATicketDispenser();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly, Category = "Scene References")
	AItemBase* TicketItem;

	UPROPERTY(EditAnywhere, Category = "Light Setup")
	USpotLightComponent* SmallOrangeLight;

	UPROPERTY(EditAnywhere, Category = "Light Setup")
	float MaxLightIntensity = 1200.0f;

	UPROPERTY(EditAnywhere, Category = "Light Setup")
	float LightInterpSpeed = 1000.0f;

	UPROPERTY(EditInstanceOnly)
	bool bShouldTurnOn = false;

	FTimerHandle LightFadeTimerHandle;
	FTimerHandle LightFlickerTimerHandle;
	float TargetLightIntensity = 0;
	bool bShouldFlickerOn = false;
	float FlickerInterval = 0.0f;

	AProjectLiminalPlayerController* LiminalPlayerController;
	AProjectLiminalCharacter* PlayerCharacter;
	UInventoryComponent* PlayerInventory;

	virtual void MovePlayerInFrontOfObject() override;
	void MakeLightFade(float DeltaTime);
	void MakeLightFlicker();

	/*GETTERS & SETTERS*/
public:
	UFUNCTION(BlueprintCallable)
	void ShouldTurnOnLight(bool ShouldTurnOn) { bShouldTurnOn = ShouldTurnOn; }
};
