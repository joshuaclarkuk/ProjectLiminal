#include "Items/CodeInputActor.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

ACodeInputActor::ACodeInputActor()
{
    PrimaryActorTick.bCanEverTick = true;

    KeypadRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("KeypadRootComponent"));
    SetRootComponent(KeypadRootComponent);

    // Initialize mesh components and attach click events
    for (int32 i = 1; i <= NumberOfButtons; i++)
    {
        UStaticMeshComponent* NewMesh = CreateDefaultSubobject<UStaticMeshComponent>(*FString("NumberMesh_" + FString::FromInt(i)));
        NewMesh->SetupAttachment(KeypadRootComponent);
        ButtonArray.Add(NewMesh);
    }
}

void ACodeInputActor::BeginPlay()
{
    Super::BeginPlay();

    for (int32 i = 1; i <= NumberOfButtons; i++)
    {
        UStaticMeshComponent* ClickableMesh = ButtonArray[i];
        if (ClickableMesh)
        {
            ClickableMesh->OnClicked.AddDynamic(this, &ACodeInputActor::OnMeshClicked);
            // Add other settings, such as collision settings here if needed
            ClickableMesh->SetGenerateOverlapEvents(true);
            ClickableMesh->SetNotifyRigidBodyCollision(true);
            ClickableMesh->SetCollisionProfileName(TEXT("BlockAllDynamic")); // Set collision profile as needed
        }
    }
}

void ACodeInputActor::OnMeshClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
    // Get the clicked mesh's index
    int32 MeshIndex = ButtonArray.Find(static_cast<UStaticMeshComponent*>(ClickedComp));

    // Add the corresponding number to the entered code array
    if (MeshIndex != INDEX_NONE)
    {
        EnteredCode.Add(MeshIndex); // Adding 1 to convert from index (0-8) to number (1-9)
        CheckEnteredCode();
        UE_LOG(LogTemp, Warning, TEXT("Number entered: %d"), MeshIndex);
    }
}

void ACodeInputActor::CheckEnteredCode()
{
    // Replace this logic with your predetermined code comparison
    // Here, it checks if the entered code matches a predetermined code (e.g., 1-2-3)
    if (EnteredCode.Num() >= 3 && EnteredCode[0] == 1 && EnteredCode[1] == 2 && EnteredCode[2] == 3)
    {
        // Code matches, perform action (open door, etc.)
        // You can add your door opening logic here
    }
}
