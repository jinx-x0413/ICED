// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HierarchyManagerInterface.h"
#include "HierarchyDataUpdate.generated.h"

/**
 * 
 */
UCLASS()
class UHierarchyDataUpdate : public UObject, public IHierarchyManagerInterface
{
	GENERATED_BODY()
	
public:
	
	virtual void UpdateHierarchyData(AActor* OwnerActor) override;

	virtual void GetComponentHierarchyRecursive(USceneComponent* InComponent, int IndentLevel);

};
