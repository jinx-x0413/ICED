// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactions/InteractionBase.h"
#include "InteractionDependency.h"

void UInteractionBase::Play(float InCurrentTime)
{
	Super::Play(InCurrentTime);

	Start();
}

void UInteractionBase::Pause()
{
	Super::Pause();
}

void UInteractionBase::Stop()
{
	Super::Stop();

	Finish();
}

void UInteractionBase::Initialize(FInteractionData InInteractionData)
{
	TargetData = InInteractionData;
}
