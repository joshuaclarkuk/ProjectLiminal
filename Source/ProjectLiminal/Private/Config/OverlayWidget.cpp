// Fill out your copyright notice in the Description page of Project Settings.


#include "Config/OverlayWidget.h"
#include "Components/TextBlock.h"

void UOverlayWidget::SetObjectNameText(FName ObjectName)
{
	// Convert FName to FText
	FText TextToDisplay = FText::FromName(ObjectName);

	// Set the text in the UTextBlock
	ObjectNameText->SetText(TextToDisplay);

	// Run display animation
}
