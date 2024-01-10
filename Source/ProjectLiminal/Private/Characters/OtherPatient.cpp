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
	if (AIController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AIController cast failed on %s"), *GetActorNameOrLabel());
	}
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



void AOtherPatient::PauseAndInitiateMovement()
{
	if (AIController)
	{
		GetWorldTimerManager().SetTimer(NavigationDelayTimerHandle, this, &AOtherPatient::MoveToTarget, 5.0f, false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AI Controller could not be found/cast on %s"), *GetActorNameOrLabel());
	}
}

void AOtherPatient::MoveToTarget()
{
	if (CurrentNavPointIndex < NavigationPoints.Num())
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetAcceptanceRadius(0.0f);
		MoveRequest.SetGoalActor(NavigationPoints[CurrentNavPointIndex]);
		FNavPathSharedPtr NavPath;

		// NavPath requires "address of" operator as it's an out parameter that is a pointer, not a const reference
		AIController->MoveTo(MoveRequest, &NavPath);

		// Display navigation path via debug spheres
		TArray<FNavPathPoint>& PathPoints = NavPath->GetPathPoints();
		for (auto& Point : PathPoints)
		{
			DrawDebugSphere(GetWorld(), Point.Location, 12.0f, 4, FColor::Green, false, 5.0f);
		}

		GetWorldTimerManager().SetTimer(NavigationCheckTimerHandle, this, &AOtherPatient::IncreaseNavIndexIfInRange, 1.0f, true);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s does not have a valid target to move to"), *GetActorNameOrLabel());
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
		UE_LOG(LogTemp, Display, TEXT("Out of Range"));
	}
}

void AOtherPatient::IncreaseNavIndexIfInRange()
{
	if (IsInRangeOfNavTarget(NavigationPoints[CurrentNavPointIndex]) && CurrentNavPointIndex < NavigationPoints.Num() -1)
	{
		UE_LOG(LogTemp, Display, TEXT("Nav target in range, increasing index"));
		CurrentNavPointIndex++;
		PauseAndInitiateMovement();
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Nav point not currently in range"));
	}
}



