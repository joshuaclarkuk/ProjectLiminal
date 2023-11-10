// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Codes/CodeComponent.h"
#include "Components/PointLightComponent.h"


// Sets default values for this component's properties
UCodeComponent::UCodeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UCodeComponent::BeginPlay()
{
	Super::BeginPlay();

	CodeIndicatorLight = GetOwner()->FindComponentByClass<UPointLightComponent>();
}

void UCodeComponent::EnterDigitToCode(int32 i)
{
	if (bIsUnlocked) { return; }

	EnteredCode.Add(i);
	CheckEnteredCode();
}

void UCodeComponent::CheckEnteredCode()
{
	// Need to check digit by digit as they are entered, as I'm using sounds rather than numerical codes
	for (int32 i = 0; i < EnteredCode.Num(); ++i)
	{
		if (EnteredCode[i] == CodeToOpen[i])
		{
			UE_LOG(LogTemp, Warning, TEXT("Correct number entered"));
			UE_LOG(LogTemp, Warning, TEXT("Element %d: %d"), i, EnteredCode[i]);

			if (i == CodeToOpen.Num() - 1)
			{
				bIsUnlocked = true;
				UE_LOG(LogTemp, Warning, TEXT("Correct code entered! Unlocked"));

				if (CodeIndicatorLight)
				{
					CodeIndicatorLight->SetLightColor(FLinearColor::Green);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Incorrect number entered. Clearing"));
			EnteredCode.Empty();
			return;
		}
	}
}

