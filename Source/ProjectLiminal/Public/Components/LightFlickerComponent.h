// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LightFlickerComponent.generated.h"

class ULightComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTLIMINAL_API ULightFlickerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULightFlickerComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Flicker Config")
	float MinFlickerTime = 0.1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Flicker Config")
	float MaxFlickerTime = 1.0f;

	ULightComponent* AffectedLight;
	float TargetLightIntensity = 0.0f;
	float CurrentFlickerInterval = 1.0f;
	float FlickerTimer = 0.0f;
	bool bLightIsActive = true;

	void SetNewLightIntensity(float DeltaTime);
	void SwitchLightOnAndOff(float DeltaTime);
};
