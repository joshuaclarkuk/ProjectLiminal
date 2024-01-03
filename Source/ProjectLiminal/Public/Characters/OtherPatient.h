// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OtherPatient.generated.h"

class USkeletalMesh;

UENUM()
enum class EPatientState {
	EPS_Waiting,
	EPS_Called,
	EPS_Travelling
};

UCLASS()
class PROJECTLIMINAL_API AOtherPatient : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOtherPatient();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PatientConfig")
	USkeletalMesh* OtherPatientMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PatientConfig")
	EPatientState PatientState;

	/*GETTERS & SETTERS*/
public:	


};
