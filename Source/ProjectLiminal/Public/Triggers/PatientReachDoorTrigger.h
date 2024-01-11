// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatientReachDoorTrigger.generated.h"

class UBoxComponent;

UCLASS()
class PROJECTLIMINAL_API APatientReachDoorTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatientReachDoorTrigger();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxTrigger;

	UPROPERTY(EditInstanceOnly, Category = "Trigger Config")
	AActor* DisplayScreenActor;

	UPROPERTY(EditInstanceOnly, Category = "Trigger Config")
	UMaterialInstance* DisplayScreenDefaultMaterial;

	UPROPERTY(EditInstanceOnly, Category = "Trigger Config")
	USoundBase* PatientCodeSuccessSound;

	UPROPERTY(EditInstanceOnly, Category = "Trigger Config")
	AActor* DoorToOpen;

	UStaticMeshComponent* ScreenMeshComponent;

	// Makes the patient wait at door for a few seconds to simulate code input
	FTimerHandle DelayForEffectTimerHandle;

	UFUNCTION()
	void OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OpenDoorAfterDelay();
	void PlaySuccessSound();
	void SetDefaultDisplayScreenMaterial();
	void OpenDoor();
};
