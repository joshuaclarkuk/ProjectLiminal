// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Interactable/InteractableComponentBase.h"

// Sets default values for this component's properties
UInteractableComponentBase::UInteractableComponentBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractableComponentBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractableComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractableComponentBase::ToggleMouseCursor(bool bShouldShowMouse)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = bShouldShowMouse;
		UE_LOG(LogTemp, Warning, TEXT("Should show mouse toggled"));
	}
}

