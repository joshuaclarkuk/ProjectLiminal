// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Codes/CodeComponent.h"

// Sets default values for this component's properties
UCodeComponent::UCodeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UCodeComponent::EnterDigitToCode(int32 i)
{
	EnteredCode.Add(i);
	for (int32 i = 0; i < EnteredCode.Num(); ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("Element %d: %d"), i, EnteredCode[i]);
	}
}

void UCodeComponent::CheckEnteredCode()
{

}

