// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CodeComponent.generated.h"

class UPointLightComponent;
class UMoveWithInterpComponent;
class AProjectLiminalCharacter;
class UInventoryComponent;

UENUM()
enum class ECodeValues {
	// NEED TO CONSTRUCT ENUM SO THAT ONLY MULTIPLES AND ADDITIONS OF 1, 3, 7, 13 CAN BE ENTERED
	ECS_One = 1,
	ECS_Three = 3,
	ECS_Seven = 7,
	ECS_Thirteen = 13
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTLIMINAL_API UCodeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCodeComponent();

	void EnterDigitToCode(int32 DigitToAddToCode);

protected:
	virtual void BeginPlay() override;

	// Array to store the entered code
	UPROPERTY(EditInstanceOnly)
	TArray<int32> CodeToOpen = { 0, 1, 2 };

	// Array to store the entered code
	UPROPERTY(VisibleInstanceOnly)
	TArray<int32> EnteredCode;

	UPROPERTY(EditInstanceOnly)
	TArray<AActor*> DoorActorReferences;

	UPROPERTY(VisibleAnywhere)
	TArray<UMoveWithInterpComponent*> DoorMovementComponents;

	UPROPERTY(VisibleAnywhere)
	AProjectLiminalCharacter* PlayerCharacter;

	UPointLightComponent* CodeIndicatorLight;

	bool bIsUnlocked = false;

	void CheckEnteredCode();

};
