// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TransformerPawn.h"
#include "CorePawn.generated.h"

UCLASS()
class UEVIEWER_API ACorePawn : public APawn
{
	GENERATED_BODY()

	// ADefaultPawn
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	ACorePawn();

	virtual void Tick(float DeltaTime) override;


	// Camera
public:

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = Camera)
	USpringArmComponent* SpringArm;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
	ATransformerPawn* TransformerPawn;

	//UPROPERTY(BlueprintreadWrite, EditAnywhere)
	//	AActor* SelectedActor; // 비교를 위해 생성
	// Check While Camera Panning
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Camera")
	bool bIsPanning;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Camera")
	float PanSpeed; // For Check or Edit in BP
	// Rotation Camera Variables
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Camera")
	float RotationSpeed;
	// Zoom Camera Variables
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Camera")
	float CamGoal;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Camera")
	float WheelSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Camera")
	float ZoomScaleFactor;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Camera")
	float InterpSpeed;

private:
	// Pan Camera Variables
	float InitialX;
	float InitialY;
	float LocationX;
	float LocationY;
	FVector CamY;
	FVector CamZ;

	bool bIsHold;

	float ClampValue;

	float Min;
	float Max;

private:

	// Using Control Panning 
	void PanCamera();

	// Using Rotating Camera
	void StartCamRotating();
	void EndCamRotating();

	// Rotating Fucntions
	void GetMouseX(float AxisValue);
	void GetMouseY(float AxisValue);

	TArray<USkeletalMeshComponent*>GetSkeletalMeshComponents(AActor* TargetActor);




	// control object
public:
	UFUNCTION(BlueprintCallable)
	void SelectActor(AActor* InActor);

	UFUNCTION(BlueprintCallable)
	void ClearSelectActor();

	UFUNCTION(BlueprintCallable)
	void SetGltfOutline(AActor* TargetActor, bool bEnable);

	UFUNCTION(BlueprintCallable)
	void SetComponentOutline(USkeletalMeshComponent* TargetComponent, bool bEnable);

	UFUNCTION(BlueprintCallable)
	void ShowGizmo();

	// Panning
	void StartCamPanning();
	void EndCamPanning();
	// Zoom Function
	void Zoom(float NewAxisValue);

	void MoveForward(float Value);
	void MoveRight(float Value);



	// Gizmo Delegate : AssetActorTransform Command
public:
	void InitializeGizmoDelegate();

};
