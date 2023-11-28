// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PressableButton.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class AInteractableBase;

UCLASS()
class PROJECTLIMINAL_API APressableButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APressableButton();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TriggerButton(int32 ButtonArrayValue);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxCollider;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ButtonMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "References")
	AInteractableBase* InteractableObject;

	FTimerHandle ButtonPushHandle;

	bool bButtonIsBeingDepressed = false;
	bool bButtonIsRising = false;
	FVector ButtonStartPosition;
	FRotator ButtonStartRotation;
	FVector ButtonDepressedPosition;
	float ButtonPushInterpSpeed = 20.0f;

	void AnimateButtonPress(float DeltaTime);
	void AnimateButtonRelease(float DeltaTime);

	/*GETTERS & SETTERS*/
public:
	void SetButtonIsRising(bool ButtonIsRising) { bButtonIsRising = ButtonIsRising; }
};
