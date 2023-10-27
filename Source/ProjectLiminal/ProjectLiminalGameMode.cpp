// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectLiminalGameMode.h"
#include "ProjectLiminalCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectLiminalGameMode::AProjectLiminalGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
