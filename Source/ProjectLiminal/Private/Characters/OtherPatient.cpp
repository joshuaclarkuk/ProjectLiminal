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
	
	MoveToNavigationTarget(KeypadNavigationTarget);
}

// Called every frame
void AOtherPatient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		FAIMoveRequest MoveRequest;

		if (TargetToMoveTo)
		{
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
			UE_LOG(LogTemp, Error, TEXT("%s has no current navigation target. Cannot navigate"), *GetActorNameOrLabel());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AI Controller could not be found/cast on %s"), *GetActorNameOrLabel());
	}
}

bool AOtherPatient::IsInRangeOfNavTarget(AActor* NavTarget, double Radius) const
{
	const double DistanceToNavTarget = (NavTarget->GetActorLocation() - GetActorLocation()).Size();
	if (DistanceToNavTarget <= Radius)
	{
		return true;
	}
	else
	{
		return false;
	}
}



