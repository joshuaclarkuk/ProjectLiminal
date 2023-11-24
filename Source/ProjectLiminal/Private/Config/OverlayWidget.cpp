// Fill out your copyright notice in the Description page of Project Settings.


#include "Config/OverlayWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Math/UnrealMathUtility.h"

void UOverlayWidget::NativeConstruct()
{
	if (ObjectNameText)
	{
		ObjectNameText->SetVisibility(ESlateVisibility::Hidden);
		TextBlockSlot = Cast<UCanvasPanelSlot>(ObjectNameText->Slot);
	}
}

void UOverlayWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	if (bShouldAnimate)
	{
		// MOVE via interpolation
		float CurrentYPosition = TextBlockSlot->GetPosition().Y;
		float TargetYPosition = 0.0f;
		float NewPosition = FMath::FInterpConstantTo(CurrentYPosition, TargetYPosition, DeltaTime, AnimationSpeed);
		TextBlockSlot->SetPosition(FVector2D(TextBlockSlot->GetPosition().X, NewPosition));

		// FADE IN via interpolation
		float NewAlphaValue = FMath::FInterpConstantTo(CurrentAlphaValue, 1.0f, DeltaTime, AnimationSpeed);
		ObjectNameText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, NewAlphaValue));

		if (CurrentYPosition >= TargetYPosition)
		{
			bShouldAnimate = false;
		}
	}

}

void UOverlayWidget::SetObjectNameText(FName ObjectName)
{
	// Make sure text starts hidden each time
	ObjectNameText->SetVisibility(ESlateVisibility::Hidden);

	// Make sure text alpha starts at zero
	ObjectNameText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

	// Set new text
	FText TextToDisplay = FText::FromName(ObjectName);
	ObjectNameText->SetText(TextToDisplay);

	// Reveal text (albeit with zero alpha value)
	ObjectNameText->SetVisibility(ESlateVisibility::Visible);

	// Run display animation
	// AnimateObjectName();
}

void UOverlayWidget::AnimateObjectName()
{
	if (TextBlockSlot)
	{
		// Set to lower position
		FVector2D NewPosition(ObjectNameText->GetRenderTransformPivot().X, -YPositionExtentValue);
		TextBlockSlot->SetPosition(NewPosition);
		
		// Trigger animation
		bShouldAnimate = true;
	}
}
