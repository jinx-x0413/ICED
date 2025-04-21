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

	SetCameraFocusComponentTransform();

}

// Interaction overriding
void UCameraInteraction::Start()
{
	if (!bIsInteracting)
	{
		bIsInteracting = true;
		Initialize(TargetData);
		SetCameraFocusToComponent();
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




void UCameraInteraction::SetCameraFocusComponentTransform()
{
	if (!GWorld)
	{
		return;
	}

	APlayerController* PC = GWorld->GetFirstPlayerController();
	if (!PC)
	{
		return;
	}

	if (APawn* TempPawn = PC->GetPawn())
	{
		// TargetActor의 중앙을 계산
		FVector ActorCenter = GetTargetActorCenter();

		//FVector TargetComponentLocation = TargetData.TargetComponent->GetComponentLocation();
		FVector ComponentCenter = TargetData.TargetComponent->Bounds.Origin;
		FVector Direction = FVector::ZeroVector;

		// x, y, z축 기준으로 컴포넌트 위치가 액터 중심을 기준으로 어떻게 떨어져 있는지 계산
		Direction.X = ComponentCenter.X - ActorCenter.X;
		Direction.Y = ComponentCenter.Y - ActorCenter.Y;
		Direction.Z = ComponentCenter.Z - ActorCenter.Z;

		// 카메라의 위치를 계산 (단순히 액터 중심 + 방향 벡터)
		FVector CameraPosition = TargetData.TargetActor->GetActorLocation() + Direction;

		// 카메라는 액터를 항상 바라보도록 설정합니다.
		FRotator CameraRotation = (TargetData.TargetActor->GetActorLocation() - CameraPosition).Rotation();

		// 카메라의 위치와 회전값을 설정
		//TargetData.TargetTransform = FTransform(CameraRotation, CameraPosition, FVector(1.f));
		TargetData.TargetTransform = FTransform(CameraRotation, ActorCenter, FVector(1.f));
		;
		// 카메라의 타겟 거리 설정 (필요에 따라 조정)
		//TargetData.TargetArmLength =700.0f;
		TargetData.TargetArmLength = Cast<ACorePawn>(TempPawn)->CameraFocusArmLength;
	}
}

// feature
void UCameraInteraction::SetCameraFocusToComponent()
{
	// Scale은 무조건 111로
	TargetData.TargetTransform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

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
		TempPawn->SetActorTransform(TargetData.TargetTransform);

		ACorePawn* TempCorePawn = Cast<ACorePawn>(TempPawn);

		if (TempCorePawn)
		{
			TempCorePawn->CamGoal = TargetData.TargetArmLength;
			TempCorePawn->SpringArm->TargetArmLength = TargetData.TargetArmLength;

		}
	}

}
