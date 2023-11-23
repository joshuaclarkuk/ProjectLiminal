// Fill out your copyright notice in the Description page of Project Settings.


#include "Config/PlayerHUD.h"
#include "Config/OverlayWidget.h"
#include "Kismet/GameplayStatics.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController && OverlayWidgetClass)
	{
		OverlayWidget = CreateWidget<UOverlayWidget>(PlayerController, OverlayWidgetClass);
		OverlayWidget->AddToViewport();
	}
}
