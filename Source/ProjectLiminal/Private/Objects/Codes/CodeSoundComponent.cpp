// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Codes/CodeSoundComponent.h"
#include "Objects/UniqueObjects/CodeMachine.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UCodeSoundComponent::UCodeSoundComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

    InitialiseCodeSoundArray();
}


// Called when the game starts
void UCodeSoundComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
    AudioComponent = GetOwner()->GetComponentByClass<UAudioComponent>();
}

// Called every frame
void UCodeSoundComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCodeSoundComponent::InitialiseCodeSoundArray()
{
    static ConstructorHelpers::FObjectFinder<USoundBase> SoundCue1(TEXT("/Game/Audio/SFX/Buttons/Alligator_Bass_Note.Alligator_Bass_Note"));
    static ConstructorHelpers::FObjectFinder<USoundBase> SoundCue2(TEXT("/Game/Audio/SFX/Buttons/Fox_Scream.Fox_Scream"));

	CodeSounds.Add(SoundCue1.Object);
	CodeSounds.Add(SoundCue2.Object);

    // Ensure the array has the correct number of elements (15 in this case)
	CodeSounds.SetNum(15);
}

void UCodeSoundComponent::PlayCodeSound(int32 CodeSoundValue)
{
    if (AudioComponent)
    {
        switch (CodeSoundValue)
        {
        case 1:
            UE_LOG(LogTemp, Warning, TEXT("Value is 1"));
            //AudioComponent->SetSound(CodeSounds[0]);
            //AudioComponent->Play();
            break;

        case 3:
            UE_LOG(LogTemp, Warning, TEXT("Value is 3"));
            break;

        case 7:
            UE_LOG(LogTemp, Warning, TEXT("Value is 7"));
            break;

        case 13:
            UE_LOG(LogTemp, Warning, TEXT("Value is 13"));
            break;

        case 4:
            UE_LOG(LogTemp, Warning, TEXT("Value is 4"));
            //AudioComponent->SetSound(CodeSounds[1]);
            //AudioComponent->Play();
            break;

        default:
            // Code to execute if Value doesn't match any case
            UE_LOG(LogTemp, Warning, TEXT("Value is not 1, 3, 7 or 13"));
            break;
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Could not find audio component on: %s for use by: %s"), *GetOwner()->GetName(), *GetName());
    }

}

void UCodeSoundComponent::StopCodeSound()
{

}

