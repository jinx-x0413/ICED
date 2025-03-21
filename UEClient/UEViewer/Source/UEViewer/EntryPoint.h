// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "EasyFileDialog/Public/EFDFunctionLibrary.h"
#include "glTFRuntime/Public/glTFRuntimeAssetActor.h"
#include "glTFRuntimeFBX/Public/glTFRuntimeFBXAssetActor.h"
#include "Service/Domain/AssetActor/public/AssetActor.h"
#include "EntryPoint.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UEVIEWER_API UEntryPoint : public UObject
{
	GENERATED_BODY()
	

	// AssetActor
public:
	UFUNCTION()
	static void LoadActor();

	UFUNCTION(BlueprintCallable)
	static void CreateGltfAssetActor(
		TSubclassOf<AGltfAssetActor> AssetActorClass
		, FTransform InTransform
		, class UglTFRuntimeAsset* InAsset
		, const FString& FilePath
		, FString InFileName);
};
