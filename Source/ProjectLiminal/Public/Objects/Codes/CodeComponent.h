// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CodeComponent.generated.h"

class UPointLightComponent;
class UMoveWithInterpComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTLIMINAL_API UCodeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCodeComponent();

	void EnterDigitToCode(int32 i);

protected:
	virtual void BeginPlay() override;

	// Array to store the entered code
	UPROPERTY(EditInstanceOnly)
	TArray<int32> CodeToOpen{ 0,1,2 };

	// Array to store the entered code
	UPROPERTY(VisibleInstanceOnly)
	TArray<int32> EnteredCode;

	UPROPERTY(EditInstanceOnly)
	TArray<AActor*> DoorActorReferences;

	UPROPERTY(VisibleAnywhere)
	TArray<UMoveWithInterpComponent*> DoorMovementComponents;

	UPointLightComponent* CodeIndicatorLight;

	bool bIsUnlocked = false;

	void CheckEnteredCode();

};
