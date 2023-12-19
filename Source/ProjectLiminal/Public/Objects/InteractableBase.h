// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableBase.generated.h"

class UStaticMeshComponent;
class UWidgetComponent;
class UBoxComponent;
class USceneComponent;
class UCameraComponent;
class USpringArmComponent;
class AProjectLiminalCharacter;


UCLASS()
class PROJECTLIMINAL_API AInteractableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableBase();

	virtual void SetInteractPromptVisibility(bool bVisible);

	UFUNCTION(BlueprintCallable)
	virtual void MovePlayerInFrontOfObject();

	UFUNCTION(BlueprintCallable)
	virtual void ReturnPlayerToFloor(AProjectLiminalCharacter* Player);

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
	UCameraComponent* InteractCamera;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* InteractCamSpringArm;

	UPROPERTY(EditAnywhere)
	float CameraSnapSpeedInSeconds = 0.1f;

	UPROPERTY(EditInstanceOnly, Category = "Interaction Config")
	bool bHasBeenSolved = false;

	/*GETTERS & SETTERS*/
public:
	UFUNCTION(BlueprintCallable)
	bool GetHasBeenSolved() { return bHasBeenSolved; }

	UFUNCTION(BlueprintCallable)
	void SetHasBeenSolved(bool HasBeenSolved) { bHasBeenSolved = HasBeenSolved; }
};
