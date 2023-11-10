// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PressableButton.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class AInteractableBase;
class UCodeComponent;
class USoundBase;

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

	UPROPERTY(EditAnywhere)
	USoundBase* SoundEffect;

	UPROPERTY(EditInstanceOnly, Category = "References")
	AInteractableBase* InteractableObject;

	UCodeComponent* CodeComponent;
	FTimerHandle ButtonPushHandle;

	bool bIsBeingDepressed = false;
	bool bIsRising = false;
	FVector ButtonStartPosition;
	FRotator ButtonStartRotation;
	FVector ButtonDepressedPosition;
	float ButtonPushInterpSpeed = 20.0f;

	void DepressButton(float DeltaTime);
	void ReturnButtonToOriginalPosition(float DeltaTime);
};
