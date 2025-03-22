// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "EasyFileDialog/Public/EFDFunctionLibrary.h"
#include "glTFRuntime/Public/glTFRuntimeAssetActor.h"
#include "glTFRuntimeFBX/Public/glTFRuntimeFBXAssetActor.h"
#include "Application/UI/Timebar/Public/Timebar.h"

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

	// Timebar
public:
	UFUNCTION(BlueprintCallable)
	static void CreateTrackToTimebar(TSubclassOf<UUserWidget> InHeaderWidget, TSubclassOf<UUserWidget> InContentWidget, FString InName);

	UFUNCTION(BlueprintCallable)
	static void DeleteTrackFromTimebar(UUserWidget* InTrackWidget);


	UFUNCTION(BlueprintCallable)
	static void CreateClipToTimebar(UUserWidget* InTrackWidget, float InStartTime, float InEndTime, FString InName);

	UFUNCTION(BlueprintCallable)
	static void DeleteClipFromTimebar(UUserWidget* InClipWidget);

	UFUNCTION(BlueprintCallable)
	static void StartTimebar();

	UFUNCTION(BlueprintCallable)
	static void PauseTimebar();

	UFUNCTION(BlueprintCallable)
	static void StopTimebar();

	UFUNCTION(BlueprintCallable)
	static void SetTimebarCurrentTime(float InCurrentTime);
};
