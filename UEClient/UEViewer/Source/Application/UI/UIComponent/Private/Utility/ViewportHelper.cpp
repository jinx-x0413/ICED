// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/ViewportHelper.h"

FVector2D UViewportHelper::AdjustedPosition(FVector2D InPosition)
{
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	float XPos = InPosition.X / ViewportSize.X;  // X 비율 (0.0 ~ 1.0)
	float YPos = InPosition.Y / ViewportSize.Y;  // Y 비율 (0.0 ~ 1.0)

	return FVector2D(XPos * ViewportSize.X, YPos * ViewportSize.Y);
}
