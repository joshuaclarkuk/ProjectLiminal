// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverlayWidget.generated.h"

class UTextBlock;
class UCanvasPanelSlot;

/**
 * 
 */
UCLASS()
class PROJECTLIMINAL_API UOverlayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ObjectNameText;

	UPROPERTY()
	UCanvasPanelSlot* TextBlockSlot;

	UPROPERTY()
	float YPositionExtentValue = 80.0f; // Determines the point on the screen the icon scrolls to and from when object is picked up

	UPROPERTY(EditAnywhere)
	float AnimationSpeed = 500.0f;

	float CurrentAlphaValue = 0.0f;
	bool bShouldAnimate = false;
	bool bHasFadedIn = false;

public:
	void SetObjectNameText(FName ObjectName);
	void AnimateObjectName();
};
