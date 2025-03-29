// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HierarchyManagerInterface.generated.h"


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

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHierarchyManagerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IHierarchyManagerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	TArray<FActorHierarchyData> HierarchyData;

	const TArray<FActorHierarchyData>& GetHierarchyData() const { return HierarchyData; }

	virtual void UpdateHierarchyData(AActor* OwnerActor) = 0;

	virtual void GetComponentHierarchyRecursive(USceneComponent* InComponent, int IndentLevel) = 0;
};
