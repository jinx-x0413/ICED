// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactions/TransformInteraction.h"
#include "InteractionDependency.h"



void UTransformInteraction::Initialize(FInteractionData InData)
{
	Super::Initialize(InData);

	if (!TargetData.TargetActor || !TargetData.TargetComponent)
	{
		return;
	}

	// TargetComponent의 위치
	FVector TargetLocation = TargetData.TargetComponent->GetComponentLocation();

	// StartTransform 설정 (현재 위치)
	TargetData.StartTransform.SetLocation(TargetLocation);
	TargetData.StartTransform.SetRotation(FQuat::Identity); // 회전 값은 필요에 따라 설정

	// EndTransform 설정 (각 축에 대해 이동 방향 설정)
	FVector Direction = FVector::ZeroVector;


	FVector Difference = TargetLocation - GetTargetActorCenter();

	// 각 축에 대한 차이의 절댓값을 계산
	float AbsX = FMath::Abs(Difference.X);
	float AbsY = FMath::Abs(Difference.Y);
	float AbsZ = FMath::Abs(Difference.Z);

	// 가장 큰 절댓값을 가진 축을 찾기
	float MaxAbsValue = FMath::Max(AbsX, FMath::Max(AbsY, AbsZ));

	// Direction을 해당 축으로 설정
	if (MaxAbsValue == AbsX) // X축이 가장 큰 차이값일 경우
	{
		Direction.X = Difference.X > 0.0f ? 200.0f : -200.0f;
		Direction.Y = 0.0f;
		Direction.Z = 0.0f;
	}
	else if (MaxAbsValue == AbsY) // Y축이 가장 큰 차이값일 경우
	{
		Direction.X = 0.0f;
		Direction.Y = Difference.Y > 0.0f ? 200.0f : -200.0f;
		Direction.Z = 0.0f;
	}
	else // Z축이 가장 큰 차이값일 경우
	{
		Direction.X = 0.0f;
		Direction.Y = 0.0f;
		Direction.Z = Difference.Z > 0.0f ? 200.0f : -200.0f;
	}

	// EndTransform 위치 계산
	TargetData.EndTransform.SetLocation(TargetLocation + Direction);
	TargetData.EndTransform.SetRotation(FQuat::Identity); // 회전 값은 필요에 따라 설정
}

void UTransformInteraction::Play(float InCurrentTime)
{
	Super::Play(InCurrentTime);

	// set current location
	if (IsValid(TargetData.TargetActor))
	{
		float NormalizedTime = FMath::Clamp((InCurrentTime - StartTime) / (EndTime - StartTime), 0.f, 1.f);
		CurrentLocation = FMath::Lerp(TargetData.StartTransform.GetLocation(), TargetData.EndTransform.GetLocation(), NormalizedTime);
	}

	Start();
}

void UTransformInteraction::Start()
{
	// set actor location
	if (IsValid(TargetData.TargetActor))
	{
		//TargetData.TargetActor->SetActorLocation(CurrentLocation);
		TargetData.TargetComponent->SetWorldLocation(CurrentLocation);
		bIsInteracting = true;
	}

}

void UTransformInteraction::Finish()
{
	CurrentLocation = FVector();
	if (UTimebarPlayer::GetTimebarPlayer()->CurrentTime <= StartTime)
	{
		TargetData.TargetComponent->SetWorldLocation(TargetData.StartTransform.GetLocation());
		//TargetData.TargetActor->SetActorLocation(TargetData.StartTransform.GetLocation());
	}
	else
	{
		TargetData.TargetComponent->SetWorldLocation(TargetData.EndTransform.GetLocation());
		//TargetData.TargetActor->SetActorLocation(TargetData.EndTransform.GetLocation());
	}
	bIsInteracting = false;
}

void UTransformInteraction::Reset()
{
	CurrentLocation = FVector();
	TargetData.TargetActor->SetActorLocation(TargetData.StartTransform.GetLocation());
}

FVector UTransformInteraction::GetTargetActorCenter()
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
