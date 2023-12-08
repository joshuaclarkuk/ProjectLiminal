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
class UTextRenderComponent;
class UNiagaraComponent;

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
	virtual void ReturnPlayerToFloor(AProjectLiminalCharacter* Player) override;

	UPROPERTY(VisibleAnywhere, Category = "CodeMachineConfig")
	UCodeComponent* CodeComponent;

	int32 NumberOfButtonCombinations = 15;

	UPROPERTY(VisibleAnywhere, Category = "CodeMachineConfig")
	USceneComponent* AudioComponentHeader;

	UPROPERTY(VisibleAnywhere, Category = "CodeMachineConfig")
	TArray<UAudioComponent*> AudioComponents;

	UPROPERTY(VisibleAnywhere, Category = "CodeMachineConfig")
	USceneComponent* CodeSymbolHeader;

	UPROPERTY(EditAnywhere, Category = "CodeMachineConfig")
	TArray<UTextRenderComponent*> CodeSymbolTextRenders;

	UPROPERTY(VisibleAnywhere, Category = "CodeMachineConfig")
	USceneComponent* CodeSymbolStaticMeshHeader;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CodeMachineConfig")
	TArray<UStaticMeshComponent*> CodeSymbolStaticMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CodeMachineConfig")
	UStaticMeshComponent* ActiveSymbolStaticMesh;

	UPROPERTY(EditAnywhere, Category = "CodeMachineConfig")
	UNiagaraComponent* KeyCodeAmbientParticles;

	UPROPERTY(EditAnywhere, Category = "CodeMachineConfig")
	UNiagaraComponent* KeyCodeSymbolParticles;

	UPROPERTY(VisibleAnywhere, Category = "CodeMachineConfig")
	UPointLightComponent* CodeIndicatorLight;

	UPROPERTY(EditDefaultsOnly, Category = "CodeMachineConfig")
	USoundWave* CorrectTicketMissingAudio;

	UPROPERTY(VisibleAnywhere, Category = "CodeMachineConfig")
	TArray<AActor*> ArrayOfAttachedButtons;

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
	void InitialiseCodeSymbolsTextRenders();
	void InitialiseCodeSymbolStaticMeshes();
	void ConstructPressableButtonArray();
	void SilenceAudioComponentsOnStart();
	void HideCodeSymbolsOnStart();
	void SelectSound(int32 CodeValue);
	void PlaySoundAndDisplayGlyph(int32 CodeValue, int32 Index);

	/*GETTERS & SETTERS*/
public:
	AItemBase* GetItemRequiredToOpen() { return ItemRequiredToOpen; }
};
