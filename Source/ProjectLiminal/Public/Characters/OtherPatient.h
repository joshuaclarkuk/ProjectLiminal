// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "OtherPatient.generated.h"

class AAIController;

UCLASS()
class PROJECTLIMINAL_API AOtherPatient : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOtherPatient();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void MoveToNextNavTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* NAVIGATION */
	UPROPERTY()
	AAIController* AIController;

	UPROPERTY(EditInstanceOnly, Category = "Navigation Points")
	TArray<AActor*> NavigationPoints;

	UPROPERTY(VisibleAnywhere, Category = "Navigation Points")
	int32 CurrentNavPointIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Navigation Points")
	double NavTargetProximity = 100.0f;

	FTimerHandle NavigationDelayTimerHandle;
	FTimerHandle NavigationCheckTimerHandle;

	bool IsInRangeOfNavTarget(AActor* NavTarget) const;
	void IncreaseNavIndexIfInRange();
};
