// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "glTFRuntimeAssetActor.h"
#include "Hierarchy/HierarchyManagerInterface.h"

#include "GltfAssetActor.generated.h"


UCLASS()
class ASSETACTOR_API AGltfAssetActor : public AglTFRuntimeAssetActor, public IHierarchyManagerInterface
{
	GENERATED_BODY()
	
	// construct
public:
	AGltfAssetActor();
	virtual ~AGltfAssetActor();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	void UpdateHierarchy();

	// Asset
public:
	void Initialize(int32 InIndex, UglTFRuntimeAsset* InAsset, FTransform InTransform, const FString& InFilePath, FString InName);
	void SetInitialBoundBoxExtent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* BoxComponent;

protected:
	UPROPERTY(VisibleAnywhere)
	class  USceneComponent* SceneComponent;

private:
	/*UPROPERTY()
	UHierarchyManager* HierarchyManager;*/

	//구조체 데이터를 직접 보유
	/*UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FActorHierarchyData> HierarchyData;*/

	// 인터페이스 포인터
	///IHierarchyManagerInterface* HierarchyManagerInterface;

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
	UPROPERTY(BlueprintReadWrite)
	TArray<FActorHierarchyData> ActorHierarchyData;

	//다른 클래스에서 hierarchydata 필요할 때 사용
	//const TArray<FActorHierarchyData>& GetHierarchyData() const { return HierarchyData; }


	//
	//void ResetHierarchyData();
	//void GetHierarchyDataRecursive(int IndentLevel);
	//void GetComponentHierarchyRecursive(USceneComponent* InComponent, int IndentLevel);

	//// Outline
	void SetOutline(bool bIsActivated);
	UFUNCTION()
	void CallbackOnActiveSelected();
		UFUNCTION()
		void CallbackOnDeactiveSelected();


	// Interface : Hierarchy Data
public:
	UFUNCTION(BlueprintCallable)
	const TArray<FActorHierarchyData>& GetInterfaceHierarchyData() const;
};
