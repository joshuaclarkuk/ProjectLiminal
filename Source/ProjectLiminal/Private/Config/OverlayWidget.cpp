// Fill out your copyright notice in the Description page of Project Settings.


#include "Config/OverlayWidget.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

void UOverlayWidget::SetObjectNameText(FName ObjectName)
{
	// Set new text
	FText TextToDisplay = FText::FromName(ObjectName);
	ObjectNameText->SetText(TextToDisplay);

	// Run display animation
	AnimateObjectName();
}

void UOverlayWidget::AnimateObjectName()
{
	if (AnimationToPlay)
	{
		PlayAnimation(AnimationToPlay);
	}
}
