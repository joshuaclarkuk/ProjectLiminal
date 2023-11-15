// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/InteractableBase.h"
#include "TicketDispenser.generated.h"

class USpotLightComponent;

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

	UPROPERTY(EditAnywhere, Category = "Light Setup")
	USpotLightComponent* SmallOrangeLight;

	UPROPERTY(EditAnywhere, Category = "Light Setup")
	float TargetLightIntensity = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Light Setup")
	float LightInterpSpeed = 100.0f;

	UPROPERTY(EditInstanceOnly)
	bool bShouldTurnOn = false;

	void TurnOnLight(float DeltaTime);

	/*GETTERS & SETTERS*/
public:
	UFUNCTION(BlueprintCallable)
	void ShouldTurnOnLight(bool ShouldTurnOn) { bShouldTurnOn = ShouldTurnOn; }
};
