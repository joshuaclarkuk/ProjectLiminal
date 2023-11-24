// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

class UStaticMeshComponent;

UCLASS()
class PROJECTLIMINAL_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditDefaultsOnly)
	FRotator RotationRate = FRotator(0.0f, 180.0f, 0.0f);

	UPROPERTY(EditAnywhere)
	FName ItemName;

public:
	void ToggleVisibilityInGame(bool IsVisible);
	FName GetItemName() { return ItemName; }
};
