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
class UCodeSoundComponent;

/**
 * 
 */
UCLASS()
class PROJECTLIMINAL_API ACodeMachine : public AInteractableBase
{
	GENERATED_BODY()
	
public:
	ACodeMachine();

	void AttemptButtonPress(int32 ButtonArrayValue);
	void RejectButtonPress();
	void EnterDigitToCode();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "CodeMachineConfig")
	UCodeComponent* CodeComponent;

	UPROPERTY(VisibleAnywhere, Category = "CodeMachineConfig")
	UCodeSoundComponent* SoundComponent;

	UPROPERTY(VisibleAnywhere, Category = "CodeMachineConfig")
	UPointLightComponent* CodeIndicatorLight;

	UPROPERTY(EditDefaultsOnly, Category = "CodeMachineConfig")
	USoundBase* CorrectTicketMissingAudio;

	UPROPERTY(VisibleAnywhere, Category = "CodeMachineConfig")
	TArray<AActor*> ArrayOfAttachedButtons;
	void ConstructPressableButtonArray();

	UPROPERTY(EditAnywhere, Category = "CodeMachineConfig")
	AItemBase* ItemRequiredToOpen;

	AProjectLiminalCharacter* PlayerCharacter;
	UInventoryComponent* InventoryComponent;

	int32 CodeValueToEnter = 0;

	/*GETTERS & SETTERS*/
public:
	AItemBase* GetItemRequiredToOpen() { return ItemRequiredToOpen; }
};
