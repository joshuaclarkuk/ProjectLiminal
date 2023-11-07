// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoveWithInterpComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTLIMINAL_API UMoveWithInterpComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoveWithInterpComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	bool bShouldMove = false;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	bool bShouldRotate = false;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	float InterpSpeed = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	FVector TargetLocation = FVector::ZeroVector;
	FVector StartLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	FRotator TargetRotation = FRotator::ZeroRotator;
	FRotator StartRotation = FRotator::ZeroRotator;

	void MoveToNewLocation(FVector TargetLocation, float DeltaTime);
	void MoveToNewRotation(FRotator TargetRotation, float DeltaTime);
};
