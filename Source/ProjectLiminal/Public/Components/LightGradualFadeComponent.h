// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LightGradualFadeComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTLIMINAL_API ULightGradualFadeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULightGradualFadeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ToggleLight(bool ShouldBeOn);

private:
	UPROPERTY(EditInstanceOnly, Category = "Config Light", meta = (AllowPrivateAccess = "true"))
	float StartingLightValue = 0.0f;

	UPROPERTY(EditInstanceOnly, Category = "Config Light", meta = (AllowPrivateAccess = "true"))
	float MaxLightValue = 8.0f;

	UPROPERTY(EditInstanceOnly, Category = "Config Light", meta = (AllowPrivateAccess = "true"))
	float FadeSpeed = 2.0f;

	UPROPERTY(VisibleInstanceOnly, Category = "Config Light", meta = (AllowPrivateAccess = "true"))
	bool bShouldBeOn = false;

	ULightComponent* LightComponentToFade;

	void FadeToNewValue(float DeltaTime);
};
