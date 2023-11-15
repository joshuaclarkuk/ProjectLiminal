// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/InteractableBase.h"
#include "VendingMachine.generated.h"

class AKeyboard;
class UCodeComponent;
class APressableButton;
class UPointLightComponent;

/**
 * 
 */
UCLASS()
class PROJECTLIMINAL_API AVendingMachine : public AInteractableBase
{
	GENERATED_BODY()
	
public:
	AVendingMachine();

	void PressButton(int32 ButtonArrayValue);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UCodeComponent* CodeComponent;

	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* CodeIndicatorLight;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> ArrayOfAttachedButtons;
	void ConstructPressableButtonArray();
};
