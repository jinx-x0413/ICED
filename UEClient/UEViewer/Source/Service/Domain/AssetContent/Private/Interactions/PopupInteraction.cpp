// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactions/PopupInteraction.h"
#include "InteractionDependency.h"

void UPopupInteraction::Start()
{
	if (!bIsInteracting)
	{
		bIsInteracting = true;

		if (IsValid(TargetWidget))
		{
			TargetData.TargetPopupWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}

}

void UPopupInteraction::Finish()
{
	if (bIsInteracting)
	{
		bIsInteracting = false;

		if (IsValid(TargetWidget))
		{
			TargetData.TargetPopupWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

}

void UPopupInteraction::Reset()
{
}
