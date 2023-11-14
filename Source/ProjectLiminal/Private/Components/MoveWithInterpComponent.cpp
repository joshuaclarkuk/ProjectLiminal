// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MoveWithInterpComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMoveWithInterpComponent::UMoveWithInterpComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMoveWithInterpComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	AActor* OwningActor = GetOwner();
	if (OwningActor)
	{
		StartLocation = OwningActor->GetActorLocation();
		StartRotation = OwningActor->GetActorRotation();
	}
}

// Called every frame
void UMoveWithInterpComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if (bShouldMove)
	{
		MoveToNewLocation(TargetLocation, DeltaTime);
	}
	else if (bShouldMoveBack)
	{
		MoveToNewLocation(StartLocation, DeltaTime);
	}

	if (bShouldRotate)
	{
		MoveToNewRotation(TargetRotation, DeltaTime);
	}
	else if (bShouldRotateBack)
	{
		MoveToNewRotation(StartRotation, DeltaTime);
	}
}

void UMoveWithInterpComponent::MoveToNewLocation(FVector MoveTo, float DeltaTime)
{
	AActor* OwningActor = GetOwner();
	if (OwningActor)
	{
		FVector CurrentLocation = OwningActor->GetActorLocation();
		FVector NewLocation = FMath::VInterpTo(CurrentLocation, MoveTo, DeltaTime, InterpSpeed);
		OwningActor->SetActorRelativeLocation(NewLocation);

		//if (NewLocation.Equals(MoveTo, 0.5f))
		//{
		//	bShouldMove = false;
		//	bShouldMoveBack = false;
		//}
	}
}

void UMoveWithInterpComponent::MoveToNewRotation(FRotator RotateTo, float DeltaTime)
{
	AActor* OwningActor = GetOwner();
	if (OwningActor)
	{
		FRotator CurrentRotation = OwningActor->GetActorRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, RotateTo, DeltaTime, InterpSpeed);
		OwningActor->SetActorRelativeRotation(NewRotation);
	}
}




