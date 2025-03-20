// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AssetActorController.generated.h"

/**
 * 
 */

class AGltfAssetActor;

UCLASS()
class UAssetActorController : public UObject
{
	GENERATED_BODY()
	
	// construct
public:
	UAssetActorController();
	virtual ~UAssetActorController();
	



	// business logic
public:
	// Create
	virtual AGltfAssetActor* CreateGltfAssetActor(
		TSubclassOf<AGltfAssetActor> AssetActorClass
		, FTransform InTransform
		, class UglTFRuntimeAsset* InAsset
		, const FString& FilePath
		, FString InFileName);

	
};
