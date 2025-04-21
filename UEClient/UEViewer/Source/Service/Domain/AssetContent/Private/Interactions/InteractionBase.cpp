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
FVector UInteractionBase::GetTargetActorCenter()
{
	if (!IsValid(TargetData.TargetActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetActor is Invalid at UTransformInteraction"));
		return FVector::ZeroVector;
	}

	FVector ActorBoundsMin(FLT_MAX, FLT_MAX, FLT_MAX);
	FVector ActorBoundsMax(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (UActorComponent* Component : TargetData.TargetActor->GetComponents())
	{
		if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(Component))
		{
			FBoxSphereBounds Bounds = PrimitiveComponent->Bounds; // 월드 기준 바운드
			FVector WorldBoundsMin = Bounds.Origin - Bounds.BoxExtent;
			FVector WorldBoundsMax = Bounds.Origin + Bounds.BoxExtent;

			ActorBoundsMin = ActorBoundsMin.ComponentMin(WorldBoundsMin);
			ActorBoundsMax = ActorBoundsMax.ComponentMax(WorldBoundsMax);
		}
	}

	return (ActorBoundsMin + ActorBoundsMax) / 2.0f;
}

