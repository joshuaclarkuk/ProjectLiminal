// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

class UStaticMeshComponent;
class UWidgetComponent;
class UBoxComponent;
class USceneComponent;
class UCameraComponent;
class USpringArmComponent;
class AProjectLiminalCharacter;
class UActorComponent;
class UInteractableComponentBase;

UCLASS()
class PROJECTLIMINAL_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ObjectMesh;

	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* InteractPrompt;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere)
	USceneComponent* CameraLockPosition;

	UPROPERTY(EditAnywhere)
	UCameraComponent* InteractCamera;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* InteractCamSpringArm;

	UPROPERTY(EditInstanceOnly)
	TSubclassOf<UActorComponent> InteractableComponentBase = nullptr;
	/*CAN'T FIGURE OUT HOW TO MAKE THESE WORK CURRENTLY*/
	UInteractableComponentBase* AttachedInteractableComponent = nullptr;

	UPROPERTY(EditAnywhere)
	float CameraSnapSpeedInSeconds = 0.1f;

	// Tracks position player camera was in before it moved
	FVector OriginalPlayerPosition = FVector::Zero();	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetInteractPromptVisibility(bool bVisible);

	virtual void MovePlayerInFrontOfObject();

	virtual void ReturnPlayerToFloor(AProjectLiminalCharacter* Player);
};
