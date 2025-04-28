// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "EasyFileDialog/Public/EFDFunctionLibrary.h"

//#include "glTFRuntime/Public/glTFRuntimeAssetActor.h"
//#include "glTFRuntimeFBX/Public/glTFRuntimeFBXAssetActor.h"

#include "Infra/Network/HTTP_Connection/Public/HTTP_Connection.h"

#include "Service/Domain/AssetActor/public/AssetActor.h"
#include "Application/Command/public/Command.h"

#include "glTFRuntime/Public/glTFRuntimeAssetActor.h"
#include "glTFRuntimeFBX/Public/glTFRuntimeFBXAssetActor.h"
#include "Application/UI/Timebar/Public/Timebar.h"
#include "Service/Domain/AssetContent/Public/AssetContent.h"
#include "Application/UI/UIComponent/Public/UIComponent.h"


#include "EntryPoint.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UEVIEWER_API UEntryPoint : public UObject
{
	GENERATED_BODY()
	



	// Http
public:
	UFUNCTION(BlueprintCallable)
	static UHttpRequest* GetHttpRequest();

	UFUNCTION(BlueprintCallable)
	static void SendUserDataHttpRequest();


	// asset actor
public:
	UFUNCTION(BlueprintCallable)
	static AGltfAssetActor* CreateGltfAssetActor(
		TSubclassOf<AGltfAssetActor> AssetActorClass
		, FTransform InTransform
		, class UglTFRuntimeAsset* InAsset
		, const FString& FilePath
		, FString InFileName);



	// command
public:
	UFUNCTION(BlueprintCallable)
	static void Undo();

	UFUNCTION()
	static void StartSetAssetActorTransform(AActor* InActor, FTransform& InTransform);

	UFUNCTION()
	static void EndSetAssetActorTransform(AActor* InActor, FTransform& InTransform);
	static void LoadActor();

	// AssetContent
public:
	UFUNCTION(BlueprintCallable)
	static UInteractionBase* CreateInteractionToTimebar(FInteractionData InInteractionData, UTrack* InTrack, float InStartTime, float InEndTime);

	UFUNCTION(BlueprintCallable)
	static void BuildAssemblyContent(AActor* InActor, TSubclassOf<UUserWidget> InTrackHeaderWidgetClass, TSubclassOf<UUserWidget> InTrackWidgetClass);

	UFUNCTION(BlueprintCallable)
	static void BuildComponentContent(AActor* InActor, TSubclassOf<UUserWidget> InTrackHeaderWidgetClass);

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

	UFUNCTION(BlueprintCallable)
	static UTimebarManager* GetTimebarManager(FName InManagerName);


	// UIComponent
public:
	UFUNCTION(BlueprintCallable)
	static UTableManager* GetTableManager(FName InManagerName);
};
