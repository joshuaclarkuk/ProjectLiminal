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
class AKeyboard;
class UCodeComponent;
class APressableButton;
class UPointLightComponent;

UCLASS()
class PROJECTLIMINAL_API AInteractableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetInteractPromptVisibility(bool bVisible);
	virtual void MovePlayerInFrontOfObject();
	virtual void ReturnPlayerToFloor(AProjectLiminalCharacter* Player);

	void PressButton(int32 ButtonArrayValue);

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

	UPROPERTY(VisibleAnywhere)
	UCodeComponent* CodeComponent;

	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* CodeIndicatorLight;

	UPROPERTY(EditAnywhere)
	float CameraSnapSpeedInSeconds = 0.1f;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> ArrayOfAttachedButtons;
	void ConstructPressableButtonArray();

};
