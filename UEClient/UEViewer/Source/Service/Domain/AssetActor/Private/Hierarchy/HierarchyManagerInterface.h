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

	virtual const TArray<FActorHierarchyData>& GetHierarchyData() const { return HierarchyData; }

	virtual void UpdateHierarchyData(AActor* OwnerActor) {
		if (!OwnerActor) return;

		// 기존 데이터 초기화
		HierarchyData.Empty();

		// 루트 노드 추가
		FActorHierarchyData NewData;
		NewData.NodeName = OwnerActor->GetName();
		NewData.DisplayName = NewData.NodeName;
		NewData.Depth = 0;
		NewData.TargetComponent = OwnerActor->GetRootComponent();
		HierarchyData.Add(NewData);

		// 자식 컴포넌트 탐색
		USceneComponent* InRootComponent = OwnerActor->GetRootComponent();
		if (InRootComponent)
		{
			GetComponentHierarchyRecursive(InRootComponent, 1);
		}

	};

	virtual void GetComponentHierarchyRecursive(USceneComponent* InComponent, int IndentLevel) {
		if (!InComponent || InComponent->GetFName() == FName("RootScene"))
		{
			return;
		}

		FActorHierarchyData NewData;
		if (Cast<USkeletalMeshComponent>(InComponent))
		{
			NewData.NodeName = InComponent->GetName();
			NewData.DisplayName = NewData.NodeName;
			NewData.Depth = IndentLevel;
			NewData.TargetComponent = InComponent;
			
			HierarchyData.Add(NewData);


			const TArray<USceneComponent*>& InChildren = InComponent->GetAttachChildren();
			for (USceneComponent* InChild : InChildren)
			{
				GetComponentHierarchyRecursive(InChild, IndentLevel + 1);
			}
		}
		else
		{
			const TArray<USceneComponent*>& InChildren = InComponent->GetAttachChildren();
			for (USceneComponent* InChild : InChildren)
			{
				GetComponentHierarchyRecursive(InChild, IndentLevel);
			}
		}
	}
	;
};
