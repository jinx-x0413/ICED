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
		return FVector();
	}

	// 액터의 모든 컴포넌트를 고려한 최소, 최대 바운드 계산
	FVector ActorBoundsMin(FLT_MAX, FLT_MAX, FLT_MAX);
	FVector ActorBoundsMax(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (UActorComponent* Component : TargetData.TargetActor->GetComponents())
	{
		if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(Component))
		{
			FVector ComponentBoundsMin, ComponentBoundsMax;
			//PrimitiveComponent->GetCollisionBounds(true, ComponentBoundsMin, ComponentBoundsMax);
			// 월드 좌표계로 변환
			FVector WorldBoundsMin = PrimitiveComponent->GetComponentTransform().TransformPosition(ComponentBoundsMin);
			FVector WorldBoundsMax = PrimitiveComponent->GetComponentTransform().TransformPosition(ComponentBoundsMax);

			// 최소, 최대 값 갱신
			ActorBoundsMin = ActorBoundsMin.ComponentMin(WorldBoundsMin);
			ActorBoundsMax = ActorBoundsMax.ComponentMax(WorldBoundsMax);
		}
	}

	// 액터의 메시 바운드를 기준으로 중앙값 계산
	return (ActorBoundsMin + ActorBoundsMax) / 2.0f;
}
