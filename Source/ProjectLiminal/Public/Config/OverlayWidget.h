// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverlayWidget.generated.h"

class UTextBlock;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class PROJECTLIMINAL_API UOverlayWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ObjectNameText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ObjectNameAnimation;

	void AnimateObjectName();

public:
	void SetObjectNameText(FName ObjectName);
};
