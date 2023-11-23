// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UOverlayWidget;

/**
 * 
 */
UCLASS()
class PROJECTLIMINAL_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<UOverlayWidget> OverlayWidgetClass;

	UPROPERTY()
	UOverlayWidget* OverlayWidget;
};
