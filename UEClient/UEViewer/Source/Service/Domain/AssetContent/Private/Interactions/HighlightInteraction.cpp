// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactions/HighlightInteraction.h"
#include "InteractionDependency.h"


void UHighlightInteraction::Start()
{
	if (!bIsInteracting && IsValid(TargetData.TargetComponent))
	{
		bIsInteracting = true;
		UPrimitiveComponent* TargetComponent = Cast<UPrimitiveComponent>(TargetData.TargetComponent);
		TargetComponent->SetRenderCustomDepth(TargetData.bIsHighlighted);
		if (TargetData.bIsHighlighted)
		{
			TargetComponent->CustomDepthStencilValue = 1;
		}
	}
}

void UHighlightInteraction::Finish()
{
	if (bIsInteracting && IsValid(TargetData.TargetComponent))
	{
		bIsInteracting = false;
		UPrimitiveComponent* TargetComponent = Cast<UPrimitiveComponent>(TargetData.TargetComponent);
		TargetComponent->SetRenderCustomDepth(!TargetData.bIsHighlighted);
	}
}

void UHighlightInteraction::Reset()
{
}
