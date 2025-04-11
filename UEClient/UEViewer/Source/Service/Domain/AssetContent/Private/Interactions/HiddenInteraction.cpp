// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactions/HiddenInteraction.h"

#include "InteractionDependency.h"


void UHiddenInteraction::Start()
{
	if (!bIsInteracting && IsValid(TargetData.TargetActor))
	{
		bIsInteracting = true;
		TargetData.TargetComponent->SetHiddenInGame(TargetData.bIsHidden);
	}
}

void UHiddenInteraction::Finish()
{

	//if (bIsInteracting && IsValid(TargetData.TargetActor))
	if (IsValid(TargetData.TargetActor))
	{
		bIsInteracting = false;

		if (UTimebarPlayer::GetTimebarPlayer()->CurrentTime <= StartTime)
		{
			TargetData.TargetComponent->SetHiddenInGame(!TargetData.bIsHidden);
		}
		else
		{
			TargetData.TargetComponent->SetHiddenInGame(TargetData.bIsHidden);
		}
	}
}

void UHiddenInteraction::Reset()
{
}
