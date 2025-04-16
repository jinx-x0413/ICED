// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactions/CameraInteraction.h"
#include "InteractionDependency.h"

void UCameraInteraction::Initialize(FInteractionData InData)
{
	Super::Initialize(InData);

	if (!TargetData.TargetActor)
	{
		return;
	}

	APlayerController* PC = GWorld->GetFirstPlayerController();

	if (APawn* TempPawn = PC->GetPawn())
	{
		// TargetActor의 중앙을 계산
		FVector ActorCenter = GetTargetActorCenter();

		FVector TargetComponentLocation = TargetData.TargetComponent->GetComponentLocation();
		FVector Direction = TargetComponentLocation - ActorCenter;

		// 카메라의 위치를 계산 (단순히 액터 중심 + 방향 벡터)
		FVector CameraPosition = TargetData.TargetActor->GetActorLocation() + Direction;

		// 카메라는 액터를 항상 바라보도록 설정합니다.
		FRotator CameraRotation = (TargetData.TargetActor->GetActorLocation() - CameraPosition).Rotation();

		// 카메라의 위치와 회전값을 설정
		TargetData.TargetTransform = FTransform(CameraRotation, CameraPosition, FVector(1.f));

		// 카메라의 타겟 거리 설정 (필요에 따라 조정)
		TargetData.TargetArmLength = 800.0f;

	}



}

// Interaction overriding
void UCameraInteraction::Start()
{
	if (!bIsInteracting)
	{
		bIsInteracting = true;
		SetPawnTransform(TargetData.TargetTransform, TargetData.TargetArmLength);
	}

}

void UCameraInteraction::Finish()
{
	if (bIsInteracting)
	{
		bIsInteracting = false;
	}

}

void UCameraInteraction::Reset()
{
}




// feature
void UCameraInteraction::SetPawnTransform(FTransform TargetTransform, float InArmLength)
{
	// Scale은 무조건 111로
	TargetTransform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	if (!GWorld)
	{
		UE_LOG(LogTemp, Warning, TEXT("No world. - UCameraInteraction"));
		return;
	}
	APlayerController* PC = GWorld->GetFirstPlayerController();
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("No First Player Controller at %s. - UCameraInteraction"), *GWorld->GetName());
		return;
	}

	if (APawn* TempPawn = PC->GetPawn())
	{
		TempPawn->SetActorTransform(TargetTransform);

		ACorePawn* TempCorePawn = Cast<ACorePawn>(TempPawn);

		if (TempCorePawn)
		{
			TempCorePawn->CamGoal = InArmLength;
			TempCorePawn->SpringArm->TargetArmLength = InArmLength;

		}
	}

}
