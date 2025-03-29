// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HierarchyManager.generated.h"


//USTRUCT(BlueprintType)
//struct FActorHierarchyData
//{
//	GENERATED_BODY()
//
//	UPROPERTY(BlueprintReadWrite)
//	FString NodeName;
//
//	UPROPERTY(BlueprintReadWrite)
//	FString DisplayName;
//
//	UPROPERTY(BlueprintReadWrite)
//	int Depth;
//	UPROPERTY(BlueprintReadWrite)
//	TWeakObjectPtr<USceneComponent> TargetComponent;
//
//};

/**
 * 
 */
UCLASS()
class UHierarchyManager : public UObject
{
	GENERATED_BODY()
	
public:
	void InitializeHierarchy(AActor* InOwner);

	//다른 클래스에서 hierarchydata 필요할 때 사용
	//const TArray<FActorHierarchyData>& GetHierarchyData() const { return HierarchyData; }

private:
	void ResetHierarchyData();
	void GetHierarchyDataRecursive(AActor* InActor, int32 IndentLevel);
	void GetComponentHierarchyRecursive(USceneComponent* InComponent, int32 IndentLevel);

//private:
//	UPROPERTY()
//	TArray<FActorHierarchyData> HierarchyData;

};
