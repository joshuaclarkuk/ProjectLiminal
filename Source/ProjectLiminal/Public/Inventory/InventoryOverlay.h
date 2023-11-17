// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryOverlay.generated.h"

class UBackgroundBlur;

UENUM()
enum class EBlurState {
	EBS_Blurring,
	EBS_Deblurring,
	EBS_Static
};

UCLASS()
class PROJECTLIMINAL_API UInventoryOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
	void ActivateOverlay(bool ShouldEnable, float DeltaTime);

protected:
	UPROPERTY(meta = (BindWidget))
	UBackgroundBlur* BackgroundBlur;

	float BlurMaxStrength = 5.0f;
	float BlurMinStrength = 0.0f;
	float BlurInterpSpeed = 23.0f;

	EBlurState BlurState = EBlurState::EBS_Static;
	
	/*GETTERS & SETTERS*/
public:
	void SetBlurState(EBlurState NewBlurState) { BlurState = NewBlurState; }
};
