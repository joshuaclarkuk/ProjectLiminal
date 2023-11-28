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
	ECS_One_Single = 1,
	ECS_Three_Single = 3,
	ECS_Seven_Single = 7,
	ECS_Thirteen_Single = 13,

	ECS_Four_Double = 4,
	ECS_Eight_Double = 8,
	ECS_Ten_Double = 10,
	ECS_Fourteen_Double = 14,
	ECS_Sixteen_Double = 16,
	ECS_Twenty_Double = 20,

	ECS_Eleven_Triple = 11,
	ECS_Seventeen_Triple = 17,
	ECS_TwentyOne_Triple = 21,
	ECS_TwentyThree_Triple = 23,

	ECS_TwentyFour_Quad = 24
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
	TArray<ECodeValues> CodeToOpen = { ECodeValues::ECS_One_Single, ECodeValues::ECS_One_Single, ECodeValues::ECS_One_Single };

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
