// Fill out your copyright notice in the Description page of Project Settings.


#include "SceneCapture/SceneCaptureIcon.h"
#include "SceneCaptureDependency.h"

// construct
void USceneCaptureIcon::NativeConstruct()
{
	Super::NativeConstruct();
}

void USceneCaptureIcon::NativeDestruct()
{
	Super::NativeDestruct();
}




// feature
void USceneCaptureIcon::SetCapturedSkeletalMeshTexture(
	USceneCaptureComponent2D* InSceneCaptureComponent
	, USkeletalMeshComponent* InComponent
	, UImage* InImageWidget)
{
	if (UTexture2D* CaptureTexture = GetSkeletalMeshCapture(InSceneCaptureComponent, InComponent))
	{
		FSlateBrush NewBrush;
		NewBrush.SetResourceObject(CaptureTexture);
		NewBrush.ImageSize = FVector2D(128.f, 128.f); // 원하면 RenderTarget 해상도 기준

		InImageWidget->SetBrush(NewBrush);
	}
}




UTexture2D* USceneCaptureIcon::GetSkeletalMeshCapture(
	USceneCaptureComponent2D* InSceneCaptureComponent,
	USkeletalMeshComponent* InComponent)
{
	if (!InSceneCaptureComponent || !InComponent || !InComponent->SkeletalMesh)
	{
		return nullptr;
	}

	// Render Target 생성 및 등록
	UTextureRenderTarget2D* RenderTarget = NewObject<UTextureRenderTarget2D>();
	RenderTarget->AddToRoot(); // GC 방지
	RenderTarget->InitAutoFormat(512, 512);
	RenderTarget->ClearColor = FLinearColor::Transparent;
	RenderTarget->UpdateResourceImmediate(true);
	InSceneCaptureComponent->TextureTarget = RenderTarget;

	// 월드 참조
	UWorld* World = GetWorld();
	if (!World) return nullptr;

	// 임시 Actor 생성
	AActor* TempActor = World->SpawnActor<AActor>();
	if (!TempActor) return nullptr;

	// Root 설정
	USceneComponent* TempRootComponent = NewObject<USceneComponent>(TempActor);
	TempActor->AddInstanceComponent(TempRootComponent);
	TempRootComponent->RegisterComponent();
	TempActor->SetRootComponent(TempRootComponent);

	// SkeletalMeshComponent 생성 및 설정
	USkeletalMeshComponent* MeshComp = NewObject<USkeletalMeshComponent>(TempActor);
	TempActor->AddInstanceComponent(MeshComp);
	MeshComp->RegisterComponent();
	MeshComp->SetSkeletalMesh(InComponent->SkeletalMesh);
	MeshComp->AttachToComponent(TempRootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	MeshComp->SetRelativeLocation(FVector::ZeroVector);
	MeshComp->SetVisibility(true);
	MeshComp->SetHiddenInGame(false);

	// 캡처 컴포넌트 위치 조정
	FVector MeshLocation = MeshComp->GetComponentLocation();
	InSceneCaptureComponent->SetWorldLocation(MeshLocation + FVector(0, 0, 200));
	InSceneCaptureComponent->SetWorldRotation(FRotator(-90.f, 0.f, 0.f));
	
	InSceneCaptureComponent->bCaptureEveryFrame = false;
	InSceneCaptureComponent->bCaptureOnMovement = false;

	// PrimitiveRenderMode 설정 및 대상 설정
	InSceneCaptureComponent->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR;
	InSceneCaptureComponent->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	InSceneCaptureComponent->ShowOnlyComponents.Empty();
	InSceneCaptureComponent->ShowOnlyComponents.Add(MeshComp);

	// 캡처 수행
	TempActor->SetActorLocation(FVector::ZeroVector);
	AdjustSceneCaptureToMesh(InSceneCaptureComponent, MeshComp); // <- 핵심 함수 호출
	InSceneCaptureComponent->CaptureScene();

	// RenderTarget → Texture2D 변환
	FTextureRenderTargetResource* RTResource = RenderTarget->GameThread_GetRenderTargetResource();
	TArray<FColor> OutBMP;
	FReadSurfaceDataFlags ReadPixelFlags(RCM_UNorm);
	ReadPixelFlags.SetLinearToGamma(false);

	RenderTarget->ClearColor = FLinearColor(0, 0, 0, 0);
	RenderTarget->RenderTargetFormat = RTF_RGBA8;
	
	// 배경 제거: 알파 수동 처리
	RTResource->ReadPixels(OutBMP, ReadPixelFlags);
	for (FColor& Pixel : OutBMP)
	{
		if (Pixel.R == 0 && Pixel.G == 0 && Pixel.B == 0)
			Pixel.A = 0;
		else
			Pixel.A = 255;
	}


	

	UTexture2D* NewTexture = UTexture2D::CreateTransient(RenderTarget->SizeX, RenderTarget->SizeY, PF_B8G8R8A8);
	if (!NewTexture) return nullptr;

	void* TextureData = NewTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(TextureData, OutBMP.GetData(), OutBMP.Num() * sizeof(FColor));
	NewTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
	NewTexture->UpdateResource();

	// 캡처 후 정리 (필요시)
	TempActor->Destroy();
	RenderTarget->RemoveFromRoot();

	return NewTexture;
}




void USceneCaptureIcon::AdjustSceneCaptureToMesh(
	USceneCaptureComponent2D* SceneCapture
	, USkeletalMeshComponent* MeshComp)
{
	if (!SceneCapture || !MeshComp) return;

	// FOV 설정 (줌 효과)
	SceneCapture->FOVAngle = 30.f;

	// 메시 중심과 크기 계산
	FBoxSphereBounds Bounds = MeshComp->Bounds;
	FVector MeshCenter = Bounds.Origin;
	float MeshRadius = Bounds.SphereRadius;

	// 시야각 기준 최적 거리 계산
	const float MarginMultiplier = 1.2f;
	float Distance = (MeshRadius / FMath::Tan(FMath::DegreesToRadians(SceneCapture->FOVAngle * 0.5f))) * MarginMultiplier;

	// 카메라 위치와 회전 설정
	FVector CameraLocation = MeshCenter + FVector(0, 0, Distance);
	SceneCapture->SetWorldLocation(CameraLocation);

	FRotator CameraRotation = (MeshCenter - CameraLocation).Rotation();
	SceneCapture->SetWorldRotation(CameraRotation);
}