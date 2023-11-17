// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryOverlay.h"
#include "Components/BackgroundBlur.h"
#include "Math/UnrealMathUtility.h"

void UInventoryOverlay::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{	
	if (BlurState == EBlurState::EBS_Static) { return; }

	if (BlurState == EBlurState::EBS_Blurring)
	{
		ActivateOverlay(true, DeltaTime);
		if (BackgroundBlur->GetBlurStrength() >= BlurMaxStrength)
		{
			BlurState = EBlurState::EBS_Static;
		}
	}
	else if (BlurState == EBlurState::EBS_Deblurring)
	{
		ActivateOverlay(false, DeltaTime);
		if (BackgroundBlur->GetBlurStrength() <= BlurMinStrength)
		{
			BlurState = EBlurState::EBS_Static;
		}
	}

	Super::NativeTick(MyGeometry, DeltaTime);
}

void UInventoryOverlay::ActivateOverlay(bool ShouldEnable, float DeltaTime)
{
	if (BackgroundBlur)
	{
		float BlurCurrentStrength = BackgroundBlur->GetBlurStrength();
		float BlurNewStrength = FMath::FInterpConstantTo(BlurCurrentStrength, ShouldEnable ? BlurMaxStrength : BlurMinStrength, DeltaTime, BlurInterpSpeed);
		BackgroundBlur->SetBlurStrength(BlurNewStrength);
	}
}
