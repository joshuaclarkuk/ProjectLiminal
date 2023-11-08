#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CodeInputActor.generated.h"

class APlayerController;
class UStaticMeshComponent;

UCLASS()
class PROJECTLIMINAL_API ACodeInputActor : public AActor
{
    GENERATED_BODY()

public:
    ACodeInputActor();

    // Array to store the entered code
    TArray<int32> EnteredCode;

protected:
    virtual void BeginPlay() override;

    // Used to attach meshes to and provide a Transform
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    USceneComponent* KeypadRootComponent;

    // Array to hold mesh components representing numbers from 1 to 9
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TArray<UStaticMeshComponent*> ButtonArray;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    int32 NumberOfButtons = 9;

    // On-click event handler for the mesh components
    UFUNCTION()
    void OnMeshClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

    // Function to check the entered code
    void CheckEnteredCode();

};