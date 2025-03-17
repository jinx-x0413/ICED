// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetActorController.h"
#include "AssetActorDependency.h"

UAssetActorController::UAssetActorController()
{

}

UAssetActorController::~UAssetActorController()
{
	if (IsValid(this) && IsRooted())
	{
		RemoveFromRoot();
		MarkAsGarbage();
	}
}

AGltfAssetActor* UAssetActorController::CreateGltfAssetActor(TSubclassOf<AGltfAssetActor> AssetActorClass, FTransform InTransform, UglTFRuntimeAsset* InAsset, const FString& FilePath, FString InFileName)
{
	// open file
	AGltfAssetActor* NewActor = GWorld->SpawnActor<AGltfAssetActor>(AssetActorClass);
	NewActor->AddToRoot();


	if (AssetActorClass->IsChildOf(AGltfAssetActor::StaticClass()))
	{
		Cast<AGltfAssetActor>(NewActor)->Initialize(0, InAsset, InTransform, FilePath, InFileName);
	}

	return NewActor;
}
