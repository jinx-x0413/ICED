// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SceneCaptureIcon.generated.h"



UCLASS()
class UICOMPONENT_API USceneCaptureIcon : public UUserWidget
{
	GENERATED_BODY()
	
	// construct
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;



	// feature
public:
	UFUNCTION(BlueprintCallable)
	virtual void SetCapturedSkeletalMeshTexture(
		USceneCaptureComponent2D* InSceneCaptureComponent
		, USkeletalMeshComponent* InComponent
		, UImage* InImageWidget);

	virtual UTexture2D* GetSkeletalMeshCapture(
		USceneCaptureComponent2D* InSceneCaptureComponent
		, USkeletalMeshComponent* InComponent);

	void AdjustSceneCaptureToMesh(USceneCaptureComponent2D* SceneCapture, USkeletalMeshComponent* MeshComp);



	// feature
public:
	UFUNCTION(BlueprintImplementableEvent)
	void ExecSetActivated(bool bIsActivated);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTrack* TargetTrack;
};
