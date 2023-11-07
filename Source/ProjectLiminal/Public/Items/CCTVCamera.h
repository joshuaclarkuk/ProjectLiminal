// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCTVCamera.generated.h"

class UStaticMeshComponent;
class UPointLightComponent;
class USoundBase;

UCLASS()
class PROJECTLIMINAL_API ACCTVCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACCTVCamera();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CameraArm;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CameraHead;

	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* CameraLight;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* TracePoint;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* PlayerSightedSound;

	AActor* PlayerActor;
	FTimerHandle TraceTimerHandle;
	float TraceInterval = 0.3f;
	float TraceDistance = 2400.0f;
	bool bPlayerPromptedToAlert = false;

	void RotateToPlayer(FVector PlayerLocation);
	void TraceForPlayer();
	bool PlayerIsInSight();
};
