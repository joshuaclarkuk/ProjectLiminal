// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/OtherPatient.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"


// Sets default values
AOtherPatient::AOtherPatient()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed = 100;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void AOtherPatient::BeginPlay()
{
	Super::BeginPlay();
	
	AIController = Cast<AAIController>(GetController());
	
	MoveToNavigationTarget(NavigationPoints[CurrentNavPointIndex]);
}

// Called every frame
void AOtherPatient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsInRangeOfNavTarget(NavigationPoints[CurrentNavPointIndex]))
	{
		CurrentNavPointIndex++;
		MoveToNavigationTarget(NavigationPoints[CurrentNavPointIndex]);
	}
}

// Called to bind functionality to input
void AOtherPatient::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AOtherPatient::MoveToNavigationTarget(AActor* TargetToMoveTo)
{
	if (AIController)
	{
		if (TargetToMoveTo)
		{
			FAIMoveRequest MoveRequest;
			MoveRequest.SetAcceptanceRadius(20.0f);
			MoveRequest.SetGoalActor(TargetToMoveTo);
			FNavPathSharedPtr NavPath;

			// NavPath requires "address of" operator as it's an out parameter that is a pointer, not a const reference
			AIController->MoveTo(MoveRequest, &NavPath);

			// Display navigation path via debug spheres
			TArray<FNavPathPoint>& PathPoints = NavPath->GetPathPoints();
			for (auto& Point : PathPoints)
			{
				DrawDebugSphere(GetWorld(), Point.Location, 12.0f, 4, FColor::Green, false, 5.0f);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s does not have a valid target to move to"), *GetActorNameOrLabel());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AI Controller could not be found/cast on %s"), *GetActorNameOrLabel());
	}
}

bool AOtherPatient::IsInRangeOfNavTarget(AActor* NavTarget) const
{
	const double DistanceToNavTarget = (NavTarget->GetActorLocation() - GetActorLocation()).Size();
	if (DistanceToNavTarget <= NavTargetProximity)
	{
		return true;
		UE_LOG(LogTemp, Display, TEXT("Is In Range"));
	}
	else
	{
		return false;
	}
}



