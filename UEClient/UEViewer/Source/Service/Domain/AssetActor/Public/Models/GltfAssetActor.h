// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "glTFRuntimeAssetActor.h"

#include "GltfAssetActor.generated.h"

USTRUCT(BlueprintType)
struct FActorHierarchyData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString NodeName;

	UPROPERTY(BlueprintReadWrite)
	FString DisplayName;

	UPROPERTY(BlueprintReadWrite)
	int Depth;
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<USceneComponent> TargetComponent;
};


UCLASS()
class ASSETACTOR_API AGltfAssetActor : public AglTFRuntimeAssetActor
{
	GENERATED_BODY()
	
	// construct
public:
	AGltfAssetActor();
	virtual ~AGltfAssetActor();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);



	// Asset
public:
	void Initialize(int32 InIndex, UglTFRuntimeAsset* InAsset, FTransform InTransform, const FString& InFilePath, FString InName);
	void SetInitialBoundBoxExtent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* BoxComponent;

protected:
	UPROPERTY(VisibleAnywhere)
	class  USceneComponent* SceneComponent;


	// feature
public:
	//// Set Transform
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform InitialTransform;

	UFUNCTION(BlueprintCallable)
	void Move(FVector InLocation);
	UFUNCTION(BlueprintCallable)
	void Rotate(FRotator InRotation);
	UFUNCTION(BlueprintCallable)
	void SetScale(FVector InScale);
	UFUNCTION(BlueprintCallable)
	void ResetTransform() { SetActorTransform(InitialTransform); }

	//// Hierarchy Data
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FActorHierarchyData> HierarchyData;

	void ResetHierarchyData();
	void GetHierarchyDataRecursive(int IndentLevel);
	void GetComponentHierarchyRecursive(USceneComponent* InComponent, int IndentLevel);

	//// Outline
	void SetOutline(bool bIsActivated);
};
