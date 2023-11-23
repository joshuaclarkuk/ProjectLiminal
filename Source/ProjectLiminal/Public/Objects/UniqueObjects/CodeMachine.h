// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/InteractableBase.h"
#include "CodeMachine.generated.h"

class AKeyboard;
class UCodeComponent;
class APressableButton;
class UPointLightComponent;
class AProjectLiminalCharacter;
class UInventoryComponent;
class AItemBase;
class USoundBase;

/**
 * 
 */
UCLASS()
class PROJECTLIMINAL_API ACodeMachine : public AInteractableBase
{
	GENERATED_BODY()
	
public:
	ACodeMachine();

	void PressButton(int32 ButtonArrayValue);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UCodeComponent* CodeComponent;

	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* CodeIndicatorLight;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* CorrectTicketMissingAudio;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> ArrayOfAttachedButtons;
	void ConstructPressableButtonArray();

	UPROPERTY(EditAnywhere)
	AItemBase* ItemRequiredToOpen;

	AProjectLiminalCharacter* PlayerCharacter;
	UInventoryComponent* InventoryComponent;


};
