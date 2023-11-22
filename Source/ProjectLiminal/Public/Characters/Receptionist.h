// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Receptionist.generated.h"

class UNiagaraComponent;

UCLASS()
class PROJECTLIMINAL_API AReceptionist : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReceptionist();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Receptionist Config")
	USceneComponent* ReceptionistSceneComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Receptionist Config")
	UStaticMeshComponent* ReceptionistBodyMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Receptionist Config")
	UNiagaraComponent* BlackSmokeEffect;
};
