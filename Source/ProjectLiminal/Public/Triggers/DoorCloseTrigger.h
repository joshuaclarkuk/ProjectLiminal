// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorCloseTrigger.generated.h"

class UBoxComponent;
class ALight;

UCLASS()
class PROJECTLIMINAL_API ADoorCloseTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorCloseTrigger();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxTrigger;

	UPROPERTY(EditInstanceOnly, Category = "Trigger Config")
	AActor* DoorToClose;

	UPROPERTY(EditInstanceOnly, Category = "Trigger Config")
	ALight* LightToDisable;

	UFUNCTION()
	void OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void CloseDoor();
	void TurnOffCallLight();

};
