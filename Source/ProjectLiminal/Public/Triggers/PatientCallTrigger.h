// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatientCallTrigger.generated.h"

class UBoxComponent;
class AOtherPatient;
class UStaticMeshComponent;
class UMaterialInstance;
class USoundBase;

UCLASS()
class PROJECTLIMINAL_API APatientCallTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatientCallTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxTrigger;

	UPROPERTY(EditInstanceOnly, Category = "Trigger Config")
	AOtherPatient* OtherPatientToCall;

	UPROPERTY(EditInstanceOnly, Category = "Trigger Config")
	AActor* DisplayScreenActor;

	UPROPERTY(EditInstanceOnly, Category = "Trigger Config")
	UMaterialInstance* PatientDoorCodeDisplay;

	UPROPERTY(EditInstanceOnly, Category = "Trigger Config")
	USoundBase* PatientCallSound;

	UFUNCTION()
	void OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void TriggerNavigation();
	void DisplayPatientCodeMaterial();
	void PlayCallNoise();

	UStaticMeshComponent* ScreenMeshComponent;

	bool bHasBeenTriggered = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
