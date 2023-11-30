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
class UAudioComponent;

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
	virtual void MovePlayerInFrontOfObject() override;

	UPROPERTY(VisibleAnywhere, Category = "CodeMachineConfig")
	UCodeComponent* CodeComponent;

	UPROPERTY(VisibleAnywhere, Category = "CodeMachineConfig")
	USceneComponent* AudioComponentHeader;

	UPROPERTY(VisibleAnywhere, Category = "CodeMachineConfig")
	TArray<UAudioComponent*> AudioComponents;

	int32 NumberOfAudioComponents = 15;

	UPROPERTY(VisibleAnywhere, Category = "CodeMachineConfig")
	UPointLightComponent* CodeIndicatorLight;

	UPROPERTY(EditDefaultsOnly, Category = "CodeMachineConfig")
	USoundWave* CorrectTicketMissingAudio;

	UPROPERTY(VisibleAnywhere, Category = "CodeMachineConfig")
	TArray<AActor*> ArrayOfAttachedButtons;
	void ConstructPressableButtonArray();

	UPROPERTY(EditAnywhere, Category = "CodeMachineConfig")
	AItemBase* ItemRequiredToOpen;

	UPROPERTY(EditAnywhere, Category = "CodeMachineConfig")
	float SoundFadeInTime = 0.2f;

	UPROPERTY(EditAnywhere, Category = "CodeMachineConfig")
	float SoundFadeOutTime = 1.0f;

	AProjectLiminalCharacter* PlayerCharacter;
	UInventoryComponent* InventoryComponent;

	int32 CodeValueToEnter = 0;

	void InitialiseAudioComponents();
	void SelectSound(int32 CodeValue);
	void PlaySound(int32 CodeValue, int32 AudioComponentIndex);

	/*GETTERS & SETTERS*/
public:
	AItemBase* GetItemRequiredToOpen() { return ItemRequiredToOpen; }
};
