// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

class UStaticMeshComponent;
class UWidgetComponent;
class UBoxComponent;

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetInteractPromptVisibility(bool bVisible);
};
